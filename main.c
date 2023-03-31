// -----------------------------------------------------------------------------
// C-Skeleton to be used with HAM Library from www.ngine.de
// -----------------------------------------------------------------------------
#include "mygbalib.h"

// -----------------------------------------------------------------------------
// Project Entry Point
// -----------------------------------------------------------------------------
int main(void)
{
	//////////////////////These stuff probably can remove //////////////////////
	// Initialize HAMlib
	ham_Init();

	// Set background mode
	ham_SetBgMode(0);

	// Initialize built-in Text-System
	ham_InitText(0);

	// Draw some text
	ham_DrawText(1, 1, "Hello World");
	////////////////////////////////////////////////////////////////////////////

	// initialise
	fillPalette();
	fillSprites();

	// drawSprite()

	// Infinite loop
	for(;;);
	
	return 0;
}

