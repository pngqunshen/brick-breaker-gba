// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include <stdbool.h>
#include "gba.h"
#include "spritesMemData.h"
#include "sprites.h"
#include "buttons.h"
#include "mygbalib.h"

// -----------------------------------------------------------------------------
// Project Entry Point
// -----------------------------------------------------------------------------
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
	for (i = 0; i < num_life; i++) {
		drawSprite(HEART, LIFE_IND + i, 224 - 16*i, 0);
	}

    // Set Handler Function for interrupts and enable selected interrupts
    REG_INT = (int)&handler;
    REG_IE |= INT_TIMER0;	// timer 0, 1 second interval
    REG_IE |= INT_TIMER1;	// timer 0, 0.05 second interval
    REG_IME = 0x1;		// Enable interrupt handling

    // Set Timer Mode (fill that section and replace TMX with selected timer number)
    REG_TM0D =	49148;		// 1 seconds for each clock cycle
    REG_TM0CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;
    REG_TM1D =	64715;		// 0.05 seconds for each clock cycle
    REG_TM1CNT |= TIMER_FREQUENCY_1024 | TIMER_ENABLE | TIMER_INTERRUPTS;

	initialise_level_one();

	// Infinite loop
	for(;;);
	
	return 0;
}

void initialise_level_one(void) 
{
	int i; // general loop variable
	// platform
	drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
	drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
	// ball
	drawSprite(BALL, BALL_IND, ball_x, ball_y);
	// brick
	for (i = 0; i < 11; i++) {
		drawSprite(BRICK_RED, BRICKS_IND + i, 16*i + 32, 32);
		drawSprite(BRICK_RED, BRICKS_IND + 11 + i, 16*i + 32, 40);
	}
	game_state = 0; // unpause game, begin countdown
}