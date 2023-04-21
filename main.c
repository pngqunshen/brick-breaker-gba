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
#include "levels.h"
#include "buttons.h"
#include "powerups.h"

// -----------------------------------------------------------------------------
// Project Entry Point
// -----------------------------------------------------------------------------

/*
  general handler that handles all interrupts
  mainly deals with timer interrupts
*/
void handler(void) 
{
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one

    // handle game state logic

    // timer2 interrupt: 0.25 seconds, 4 hz
    if ((REG_IF & INT_TIMER2) == INT_TIMER2)
    {
        // set the next state for ball blinking for powerup B
        B_blink_state++;
        B_blink_state %= 4;
    }

    // timer1 interrupt: 0.05 seconds, 20 hz
    if ((REG_IF & INT_TIMER1) == INT_TIMER1)
    {
        checkbutton(); // button update
        
        switch (game_state)
        {
            case GAME_MENU:
            case GAME_MENU_LEVEL:
                mainMenu();
                break;

            case GAME_STARTING: {
                int i;
                // remove pause message
                for (i=0; i<6; i++) {
                    removeFromScreen(GAME_MESSAGE_IND+i);
                }
                break;
            }

            case GAME_STARTED:
                // check if can go next level/game won, else move
                if (total_bricks > 0) {
                    moveBall();
                } else if (current_level < 2) {
                    game_state = GAME_NEXT;
                } else {
                    game_state = GAME_WON;
                }
                break;

            case GAME_PAUSED: {
                // draw pause message
                removeFromScreen(TIMER_START_IND);
                drawSprite(LETTER_P, GAME_MESSAGE_IND, 96, 72);
                drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 104, 72);
                drawSprite(LETTER_U, GAME_MESSAGE_IND+2, 112, 72);
                drawSprite(LETTER_S, GAME_MESSAGE_IND+3, 120, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+4, 128, 72);
                drawSprite(LETTER_D, GAME_MESSAGE_IND+5, 136, 72);
                break;
            }

            case GAME_ENDING: {
                // allow falling animation to complete
                if ((ball_y+8) < SCREEN_HEIGHT) {
                    moveBall();
                } else {
                    game_state = GAME_ENDED;
                }
                break;
            }

            case GAME_ENDED: {
                // check if there is life left, if so continue, else gameover
                if (num_life > 1) {
                    num_life -= 1;
                    drawHeart();
                    ball_x = BALL_START_X;
                    ball_y = BALL_START_Y;
                    ball_heading = BALL_START_HEAD;
                    platform_x = 120;
                    drawSprite(BALL, BALL_IND, ball_x, ball_y);
                    drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
                    drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
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
                // win currrent level message
                drawSprite(LETTER_L, GAME_MESSAGE_IND, 84, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+1, 92, 72);
                drawSprite(LETTER_V, GAME_MESSAGE_IND+2, 100, 72);
                drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 108, 72);
                drawSprite(LETTER_L, GAME_MESSAGE_IND+4, 116, 72);
                drawSprite(LETTER_W, GAME_MESSAGE_IND+5, 132, 72);
                drawSprite(LETTER_O, GAME_MESSAGE_IND+6, 140, 72);
                drawSprite(LETTER_N, GAME_MESSAGE_IND+7, 148, 72);
                // move ball out of screen so it does not block message
                drawSprite(BALL, BALL_IND, SCREEN_WIDTH, SCREEN_HEIGHT);
                break;
            }

            case GAME_OVER: {
                gameOver();
                break;
            }
            
            case GAME_WON: {
                gameWon();
                break;
            }
            default:
                break;
        }
    }

    // timer0 interrupt: 1 second, 1 hz
    if ((REG_IF & INT_TIMER0) == INT_TIMER0)
    {
        switch (game_state)
        {
        case GAME_MENU:
        case GAME_MENU_LEVEL: 
        case GAME_OVER: 
        case GAME_WON: {
            main_menu_flash = !main_menu_flash;
            break;
        }

        case GAME_STARTING: {
            drawSprite(NUMBER_ZERO + start_timer, TIMER_START_IND, 116, 76);
            if (start_timer <= 0) { // countdown completed
                game_state = GAME_STARTED;
                removeFromScreen(TIMER_START_IND);
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
            powerupB_handler();
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
            drawSprite(NUMBER_ZERO + next_level_timer, TIMER_NEXT_LEVEL_IND, 116, 60);
            next_level_timer -=1 ;
            if (next_level_timer < 0) {
                initialise();
                initialiseLevelTwo();
                current_level = 2;
                int i;
                for (i=0; i<8; i++) {
                    removeFromScreen(GAME_MESSAGE_IND+i);
                }
                removeFromScreen(TIMER_NEXT_LEVEL_IND);
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
    // Set Mode 2
    *(unsigned short *) 0x4000000 = 0x40 | 0x2 | 0x1000;

    ////////////////////////////////////////////////////////////////////////////
    // initialise
    ////////////////////////////////////////////////////////////////////////////
    fillPalette();
    fillSprites();
    initialise();

    // Set Handler Function for interrupts and enable selected interrupts
    REG_INT = (int)&handler;
    REG_IE |= INT_TIMER0;	// timer 0, 1 second interval
    REG_IE |= INT_TIMER1;	// timer 1, 0.05 second interval
    REG_IE |= INT_TIMER2;	// timer 1, 0.05 second interval
    REG_IME = 0x1;		// Enable interrupt handling

    // Set Timer Mode
    REG_TM0D =	49148;		// 1 seconds for each clock cycle
    REG_TM0CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;
    REG_TM1D =	64715;		// 0.05 seconds for each clock cycle
    REG_TM1CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;
    REG_TM2D =	61438;		// 0.25 seconds for each clock cycle
    REG_TM2CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

    // Infinite loop
    for(;;);
    
    return 0;
}
