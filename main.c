// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "gba.h"
#include "config.h"
#include "sprites.h"
#include "mygbalib.h"
#include "buttons.h"
#include "levels.h"

// -----------------------------------------------------------------------------
// Project Entry Point
// -----------------------------------------------------------------------------

void handler(void) 
{
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one

    // handle game state logic
    if ((REG_IF & INT_TIMER1) == INT_TIMER1)
    {
        checkbutton(); // button update
        
        switch (game_state)
        {
            case GAME_MENU:
                mainMenu();
                break;

            case GAME_STARTED:
                if (bricks_eliminated < 27) {
                moveBall();
                } else if (bricks_eliminated == 27) {
                    game_state = GAME_NEXT;
                } else if (bricks_eliminated == 72) {
                    game_state = GAME_WON;
                }
                break;

            case GAME_ENDING:
                if (ball_y < 160) {
                    moveBall();
                } else {
                    game_state = GAME_ENDED; 
                }
                break;

            case GAME_PAUSED: {
                drawSprite(NUMBER_ZERO, TIMER_START_IND, 240, 160);
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

            case GAME_ENDED: {
                if (num_life > 1) {
                    num_life -= 1;
                    drawHeart();
                    ball_x = BALL_START_X;
                    ball_y = BALL_START_Y;
                    ball_heading = BALL_START_HEAD;
                    drawSprite(BALL, BALL_IND, ball_x, ball_y);
                    start_timer = GAME_START_COUNTDOWN;
                    game_state = GAME_STARTING;
                } else {
                    num_life -= 1;
                    drawHeart();
					game_state = GAME_OVER;
				}
				break;
            }

            case GAME_NEXT: {
                drawSprite(LETTER_L, GAME_MESSAGE_IND, 76, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+1, 84, 72);
                drawSprite(LETTER_V, GAME_MESSAGE_IND+2, 92, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 100, 72);
                drawSprite(LETTER_L, GAME_MESSAGE_IND+4, 108, 72);
                drawSprite(LETTER_W, GAME_MESSAGE_IND+5, 124, 72);
                drawSprite(LETTER_O, GAME_MESSAGE_IND+6, 132, 72);
                drawSprite(LETTER_N, GAME_MESSAGE_IND+7, 140, 72);
                break;
            }

			case GAME_OVER: {
				drawSprite(LETTER_G, GAME_MESSAGE_IND, 88, 72);
				drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 96, 72);
				drawSprite(LETTER_M, GAME_MESSAGE_IND+2, 104, 72);
				drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 112, 72);
				drawSprite(LETTER_O, GAME_MESSAGE_IND+4, 120, 72);
				drawSprite(LETTER_V, GAME_MESSAGE_IND+5, 128, 72);
				drawSprite(LETTER_E, GAME_MESSAGE_IND+6, 136, 72);
				drawSprite(LETTER_R, GAME_MESSAGE_IND+7, 144, 72);
                break;
			}
            
            case GAME_WON: {
                drawSprite(LETTER_G, GAME_MESSAGE_IND, 88, 72);
				drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 96, 72);
				drawSprite(LETTER_M, GAME_MESSAGE_IND+2, 104, 72);
				drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 112, 72);
				drawSprite(LETTER_W, GAME_MESSAGE_IND+5, 128, 72);
				drawSprite(LETTER_O, GAME_MESSAGE_IND+6, 136, 72);
				drawSprite(LETTER_N, GAME_MESSAGE_IND+7, 144, 72);
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
        case GAME_MENU: {
            main_menu_flash = !main_menu_flash;
            break;
        }

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
			if (timer < 0) {
				game_state = GAME_OVER;
			}
            break;
        }
        
        case GAME_PAUSED: {
            pause_timer -= 1;
            break;
        }
        
        case GAME_NEXT: {
            drawSprite(NUMBER_ZERO + level2_timer, TIMER_LEVEL2_IND, 116, 60);
            level2_timer-=1;
            if (level2_timer < 0) {
                initialiseLevelTwo();
                game_state = GAME_STARTED;
                drawSprite(LETTER_L, GAME_MESSAGE_IND, 240, 160);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+1, 240, 160);
                drawSprite(LETTER_V, GAME_MESSAGE_IND+2, 240, 160);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 240, 160);
                drawSprite(LETTER_L, GAME_MESSAGE_IND+4, 240, 160);
                drawSprite(LETTER_W, GAME_MESSAGE_IND+5, 240, 160);
                drawSprite(LETTER_O, GAME_MESSAGE_IND+6, 240, 160);
                drawSprite(LETTER_N, GAME_MESSAGE_IND+7, 240, 160);
            }
            break;
        }
        default:
            break;
        }
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling;
}

int main(void)
{
	//////////////////////These stuff probably can remove //////////////////////
	// Initialize HAMlib
	// ham_Init();

	// Set background mode
	// ham_SetBgMode(0);

	// Initialize built-in Text-System
	// ham_InitText(0);

	// Draw some text
	// ham_DrawText(1, 1, "Hello World");
	////////////////////////////////////////////////////////////////////////////

    // Set Mode 2
    *(unsigned short *) 0x4000000 = 0x40 | 0x2 | 0x1000;

	////////////////////////////////////////////////////////////////////////////
	// initialise
	////////////////////////////////////////////////////////////////////////////
	fillPalette();
	fillSprites();
	int i; // general loop variable
	// create walls
	for (i = 0; i < 8; i++) {
		drawSprite(LEFT_WALL, LEFT_WALL_IND + i, 0, i*16+32);
		drawSprite(RIGHT_WALL, RIGHT_WALL_IND + i, 224, i*16+32);
	}
	for (i = 0; i < 15; i++) {
		drawSprite(TOP_WALL, TOP_WALL_IND + i, 16*i, 16);
	}
	// hearts
	drawHeart();

    // Set Handler Function for interrupts and enable selected interrupts
    REG_INT = (int)&handler;
    REG_IE |= INT_TIMER0;	// timer 0, 1 second interval
    REG_IE |= INT_TIMER1;	// timer 1, 0.05 second interval
    REG_IME = 0x1;		// Enable interrupt handling

    // Set Timer Mode
    REG_TM0D =	49148;		// 1 seconds for each clock cycle
    REG_TM0CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;
    REG_TM1D =	64715;		// 0.05 seconds for each clock cycle
    REG_TM1CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

	// Infinite loop
	for(;;);
	
	return 0;
}
