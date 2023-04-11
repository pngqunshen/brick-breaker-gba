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
        drawSprite(0, i, 240,160);
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
    if (ball_bottom > BALL_PLATFORM_BOUND) {
        if ((game_state == GAME_STARTED) && (xc < platform_x + 16) && (xc >= platform_x - 16)) {
            double ang_extra = (xc - platform_x + 0.5) / 15.5 * PLATFORM_MAX_ANGLE;
            double reflection = limit_angle(-ball_heading + ang_extra);
            if ((reflection < -M_PI + M_PI/9) || (reflection > M_PI/2)) {
                ball_heading = -M_PI + M_PI/9; // offset slightly so it is not horizontal
            } else if (reflection > -M_PI/9) {
                ball_heading = -M_PI/9; // offset slightly so it is not horizontal
            } else {
                ball_heading = reflection;
            }
            return true;
        } else {
            game_state = GAME_ENDING;
        }
    }
    
    // bricks collision IMPLEMENT HERE
    // int row = sizeof(brick_hp); //hori_edges is an array that contains y-values of top and bottom edges of bricks (top,bttom,top,bottom...)
    // int col = sizeof(brick_hp[0]); //verti_edges contains x-values of left edges of bricks (left,right,left,right....)
    // for (int i = 0; i < row; i++) {
    //     if (hori_edges[i*2] == ball_bottom || hori_edges[i*2+1] == ball_top) { //check if ball_bottom coincides with any top edges in y-coord || top with bottom edges (confirm when ball clips in brick)
    //         brick_col   = (xc- leftmargin)/16 //to find out which column of bricks ball cooincides with
    //         if (brick_col < col) { //if brick col > (number of bricks) nothing happens
    //             if (brick_hp[i][brick_col] > 0) { //brick_hp is global array matrix of the hp of all brick
    //                 if ((i%2 0) == 0) { //check if it is top edge or bottom edge
    //                     ball_heading = M_PI - ball_heading //reflect upwards
    //                 } else {
    //                     ball_heading = -M_PI - ball_heading // reflect down
    //                 }
    //                 brick_hp[i][brick_col] -= 1
    //             }    
    //         }
    //     }    
    // }
    // for (int i = 0; i < col; i++) {
    //     if (verti_edges[i*2] == ball_right ||verti_edges[i*2+1] == ball_left) { //check if ball_right coincides with any left edges in x-coord || left with right edges
    //         brick_row = (yc- topmargin)/8 //to find out which row of bricks ball cooincides with
    //         if (brick_row > (row)) { //if brick row > (number of bricks) nothing happens
    //             if (brick_hp[brick_row][i] > 0) { //brick_hp is global array matrix of the hp of all brick
    //                 ball_heading = -ball_heading //reflect upwards
    //                 brick_hp[brick_row][i] -=1
    //             }
    //         }    
    //     }    
    // }
    return false;
}

void powerupA_handler() {
	if (powerupA_active) {
        int ones = powerupA_timer % 10;
        int tens = (powerupA_timer / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_POWERUP_IND, 56, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_POWERUP_IND + 1, 48, 0);
		powerupA_timer--;
		drawSprite(POWERUP_A, POWERUP_IND, 32, 0);
		if (powerupA_timer <= 0) {
			step_size /= 2; // revert platform step size
			powerupA_active = false;
			powerupA_cooldown = POWERUP_A_COOLDOWN_DURATION; // set cooldown
            drawSprite(NUMBER_ZERO, TIMER_POWERUP_IND, 240, 120);
            drawSprite(NUMBER_ZERO, TIMER_POWERUP_IND+1, 240, 120);
            drawSprite(POWERUP_A, POWERUP_IND, 240, 120);
		}
	} else if (powerupA_cooldown > 0) {
        int ones = powerupA_cooldown % 10;
        int tens = (powerupA_cooldown / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_COOLDOWN_IND, 56, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_COOLDOWN_IND + 1, 48, 0);
        powerupA_cooldown--;
        if (powerupA_cooldown <= 0) {
            drawSprite(NUMBER_ZERO, TIMER_COOLDOWN_IND, 240, 120);
            drawSprite(NUMBER_ZERO, TIMER_COOLDOWN_IND+1, 240, 120);
        }
	}
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
