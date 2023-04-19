void fillPalette(void)
{
    int     i;

	// Fill the palette in GBA memory
    for (i = 0; i < NCOLS; i++)
        spritePal[i] = palette[i];
}


void drawSprite(int numb, int N, int x, int y)
{
	// Same as CA2, make specific sprite (based on its name/numb) appear on screen, as slide number N (each sprite needs a different, arbitrary, N >= 0)
    *(unsigned short *)(0x7000000 + 8*N) = y | 0x2000;
    *(unsigned short *)(0x7000002 + 8*N) = x | 0x4000;
    *(unsigned short *)(0x7000004 + 8*N) = numb*8;
}

void fillSprites(void)
{
    int     i;

	// Load all sprites in GBA memory
    for (i = 0; i < 128*16*16; i++)
        spriteData[i] = (sprites[i*2+1] << 8) + sprites[i*2];

	// draw all sprites on screen, but all of them outside of the screen (starting at position (240,160) the bottom right corner of the GBA screen)
    for(i = 0; i < 128; i++)
        drawSprite(0, i, SCREEN_WIDTH,SCREEN_HEIGHT);
}

void removeFromScreen(int i) {
    drawSprite(0,i,SCREEN_WIDTH,SCREEN_HEIGHT);
}

void drawHeart() {
    int i;
    for (i = 0; i < MAX_NUM_LIFE; i++) {
		if (i < num_life) {
			drawSprite(HEART, LIFE_IND + i, 224 - 16*i, 0);
		} else {
			drawSprite(EMPTY_HEART, LIFE_IND + i, 224 - 16*i, 0);
		}
	}
}

void drawBrick(int x, int y, int i) {
    switch (brick_health[i])
    {
    case 1:
        drawSprite(BRICK_RED, BRICKS_IND + i, x, y);
        break;

    case 2:
        drawSprite(BRICK_YELLOW, BRICKS_IND + i, x, y);
        break;

    case 3:
        drawSprite(BRICK_GREEN, BRICKS_IND + i, x, y);
        break;

    default:
        removeFromScreen(BRICKS_IND+i);
        bricks[i][0] = SCREEN_WIDTH;
        bricks[i][1] = SCREEN_HEIGHT;
        break;
    }
}

/*
  ensure angle a is between [-pi, pi)
*/
double limit_angle(double a) {
    if (a >= -M_PI && a < M_PI) {
        return a;
    } else if (a < -M_PI) {
        return limit_angle(a + 2*M_PI);
    } else {
        return limit_angle(a - 2*M_PI);
    }
}

void brickBreak(int i) {
    brick_health[i] -= ball_dmg;
    int xb = bricks[i][0]-BRICK_LENGTH/2;
	int yb = bricks[i][1]-BRICK_HEIGHT/2;
    if (brick_health[i] <=0)
        bricks_eliminated += 1;
    drawBrick(xb, yb, i);
}

/*
  Ball is drawn at the center of 16x16 sprite, which means that
  the offset have to be considered for collision detection
  +8: to find center of ball
  +-BALL_RADIUS: to find edge of ball
*/
bool checkCollision(int x0, int y0)
{
    // set ball key pixel index
    int xc = x0 + 8;
    int yc = y0 + 8;
    int ball_top = yc - BALL_RADIUS;
    int ball_bottom = yc + BALL_RADIUS - 1;
    int ball_left = xc - BALL_RADIUS;
    int ball_right = xc + BALL_RADIUS - 1;

    // main boundaries collision
    if (ball_right > BALL_RIGHT_BOUND) {
        ball_heading = limit_angle(M_PI - ball_heading);
        return true;
    }
    if (ball_left < BALL_LEFT_BOUND) {
        ball_heading = limit_angle(M_PI - ball_heading);
        return true;
    }
    if (ball_top < BALL_UPPER_BOUND) {
        ball_heading = limit_angle(-ball_heading);
        return true;
    }

    // platform collision
    // collide with top of platform
    if (ball_bottom > BALL_PLATFORM_BOUND && ball_bottom <= (BALL_PLATFORM_BOUND+PLATFORM_HEIGHT)) {
        switch (game_state)
        {
        case GAME_STARTED: {
            if ((xc < platform_x + PLATFORM_WIDTH/2) && (xc >= platform_x - PLATFORM_WIDTH/2)) {
                double ang_extra = (xc - platform_x) / 16.0 * PLATFORM_MAX_DEFLECTION;
                double reflection = limit_angle(-ball_heading + ang_extra);
                if ((reflection < -M_PI + PLATFORM_MIN_ANGLE) || (reflection > M_PI/2)) {
                    ball_heading = -M_PI + PLATFORM_MIN_ANGLE; // offset slightly so it is not horizontal
                } else if (reflection > -PLATFORM_MIN_ANGLE) {
                    ball_heading = -PLATFORM_MIN_ANGLE; // offset slightly so it is not horizontal
                } else {
                    ball_heading = reflection;
                }
                return true;
            } else {
                game_state = GAME_ENDING;
            }
            break;
        }

        case GAME_ENDING: {
            // collide with left of platform
            if (yc > BALL_PLATFORM_BOUND && yc <= (BALL_PLATFORM_BOUND+PLATFORM_HEIGHT)) {
                if (ball_right >= (platform_x-PLATFORM_WIDTH/2) && ball_right < platform_x) {
                    // if already bouncing away, do nothing, prevent glitch where ball gets stuck
                    if (ball_heading <= M_PI/2) {
                        ball_heading = limit_angle(M_PI - ball_heading);
                        return true;
                    }
                }
                // collide with right of platform
                else if (ball_left >= platform_x && ball_left < (platform_x+PLATFORM_WIDTH/2)) {
                    // if already bouncing away, do nothing, prevent glitch where ball gets stuck
                    if (ball_heading > M_PI/2) {
                        ball_heading = limit_angle(M_PI - ball_heading);
                        return true;
                    }
                }
            }
            break;
        }
        
        default:
            break;
        }
    }
    
    // bricks collision
    int i; // general loop variable
    for (i = 0; i<BRICK_MAX_NUM; i++) {
        if (bricks[i][0] == -1 && bricks[i][1] == -1) {
            break;
        }
        // ball collide with top or bottom of brick
        if ((xc>=(bricks[i][0]-BRICK_LENGTH/2-BRICK_THRESHOLD)) && 
                (xc<(bricks[i][0]+BRICK_LENGTH/2+BRICK_THRESHOLD))) {
            if ((yc >= (bricks[i][1]-BRICK_HEIGHT/2-BALL_RADIUS)) && 
                    (yc < (bricks[i][1]+BRICK_HEIGHT/2+BALL_RADIUS))) {
                ball_heading = limit_angle(-ball_heading);
                brickBreak(i);
                return true;
            }
        }
        // ball collide left or right of brick
        if ((yc>=(bricks[i][1]-BRICK_HEIGHT/2-BRICK_THRESHOLD)) && 
                (yc<(bricks[i][1]+BRICK_HEIGHT/2+BRICK_THRESHOLD))) {
            if ((xc >= (bricks[i][0]-BRICK_LENGTH/2-BALL_RADIUS)) && 
                    (xc < (bricks[i][0]+BRICK_LENGTH/2+BALL_RADIUS))) {
                ball_heading = limit_angle(M_PI - ball_heading);
                brickBreak(i);
                return true;
            }
        }
    }

    return false;
}

/*
  move using Bresenham line algorithm
*/
void moveBall(void)
{
    int x0 = ball_x;
    int y0 = ball_y;
    int x1 = x0 + (int)(BALL_STEP_SIZE*cos(ball_heading));
    int y1 = y0 + (int)(BALL_STEP_SIZE*sin(ball_heading));
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    while (true) {
        if (checkCollision(x0, y0)) {
            break;
        }
        drawSprite(BALL, BALL_IND, ball_x, ball_y);
        ball_x = x0;
        ball_y = y0;
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = error*2;
        if (e2 >= dy) {
            if (x0 == x1) {
                break;
            }
            error += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) {
                break;
            }
            error += dx;
            y0 += sy;
        }
    }
}
