#define INPUT                      (KEY_MASK & (~REG_KEYS))

void handler(void) 
{
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one

    // handle game state logic
    if ((REG_IF & INT_TIMER1) == INT_TIMER1)
    {
        checkbutton(); // button update
        
        switch (game_state)
        {
            case GAME_STARTED:
                moveBall();
                break;

            case GAME_ENDING:
                if (ball_y < 160) {
                    moveBall();
                } else {
                    game_state = GAME_ENDED;
                }
                break;

            case GAME_PAUSED: {
                drawSprite(LETTER_P, GAME_MESSAGE_IND, 96, 72);
                drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 104, 72);
                drawSprite(LETTER_U, GAME_MESSAGE_IND+2, 112, 72);
                drawSprite(LETTER_S, GAME_MESSAGE_IND+3, 120, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+4, 128, 72);
                drawSprite(LETTER_D, GAME_MESSAGE_IND+5, 136, 72);
                break;
            }

            case GAME_STARTING: {
                drawSprite(LETTER_P, GAME_MESSAGE_IND, 240, 160);
                drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 240, 160);
                drawSprite(LETTER_U, GAME_MESSAGE_IND+2, 240, 160);
                drawSprite(LETTER_S, GAME_MESSAGE_IND+3, 240, 160);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+4, 240, 160);
                drawSprite(LETTER_D, GAME_MESSAGE_IND+5, 240, 160);
                break;
            }
            
            default:
                break;
        }
    }

    // timer
    if ((REG_IF & INT_TIMER0) == INT_TIMER0)
    {
        switch (game_state)
        {
        case GAME_STARTING: {
            drawSprite(NUMBER_ZERO + start_timer, TIMER_START_IND, 116, 76);
            if (start_timer <= 0) { // countdown completed
                game_state = GAME_STARTED;
                drawSprite(NUMBER_ZERO, TIMER_START_IND, 240, 160);
            }
            start_timer -= 1;
            pause_timer -= 1;
            break;
        }

        case GAME_STARTED:
        case GAME_ENDING: {
            int ones = timer % 10;
            int tens = (timer / 10) % 10;
            int hundreds = (timer / 100) % 10;
            drawSprite(NUMBER_ZERO + hundreds, TIMER_OVERALL_IND, 0, 0);
            drawSprite(NUMBER_ZERO + tens, TIMER_OVERALL_IND + 1, 8, 0);
            drawSprite(NUMBER_ZERO + ones, TIMER_OVERALL_IND + 2, 16, 0);
            timer -= 1;
            powerupA_handler();
            break;
        }
        
        case GAME_PAUSED: {
            pause_timer -= 1;
            break;
        }
        
        default:
            break;
        }
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling;
}

void fillPalette(void)
{
    int     i;

	// Fill the palette in GBA memory
    for (i = 0; i < NCOLS; i++)
        spritePal[i] = palette[i];
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


void drawSprite(int numb, int N, int x, int y)
{
	// Same as CA2, make specific sprite (based on its name/numb) appear on screen, as slide number N (each sprite needs a different, arbitrary, N >= 0)
    *(unsigned short *)(0x7000000 + 8*N) = y | 0x2000;
    *(unsigned short *)(0x7000002 + 8*N) = x | 0x4000;
    *(unsigned short *)(0x7000004 + 8*N) = numb*8;
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

/*
  ensure angle a is between [-pi, pi)
*/
double limit_angle(double a) {
    if (a >= -M_PI && a<M_PI) {
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
            if (reflection > 0) {
                if (reflection > M_PI/2) {
                    ball_heading = -M_PI+0.1;
                } else {
                    ball_heading = -0.1;
                }
            } else {
                ball_heading = reflection;
            }
            return true;
        } else {
            game_state = GAME_ENDING;
        }
    }

    // bricks collision IMPLEMENT HERE

    return false;
}
