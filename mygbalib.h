#define INPUT                      (KEY_MASK & (~REG_KEYS))

void handler(void) 
{
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one
    checkbutton();

    // timer
    if ((REG_IF & INT_TIMER0) == INT_TIMER0)
    {
        if (game_state == GAME_STARTING) { // give a countdown before starting game
            drawSprite(NUMBER_ZERO + GAME_START_COUNTDOWN - timer, TIMER_IND + 3, 116, 76);
            timer += 1;
            if (timer > GAME_START_COUNTDOWN + 1) { // countdown completed
                timer = 0;
                game_state = GAME_STARTED;
                drawSprite(NUMBER_ZERO, TIMER_IND + 3, 240, 160);
            }
        } else if (game_state == GAME_STARTED) { // start game normally
            int ones = timer % 10;
            int tens = (timer / 10) % 10;
            int hundreds = (timer / 100) % 10;
            drawSprite(NUMBER_ZERO + hundreds, TIMER_IND, 0, 0);
            drawSprite(NUMBER_ZERO + tens, TIMER_IND + 1, 8, 0);
            drawSprite(NUMBER_ZERO + ones, TIMER_IND + 2, 16, 0);
            timer += 1;
        }
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling;
}

void checkbutton(void)
{
	// Gift function to show you how a function that can be called upon button interrupt to detect which button was pressed and run a specific function for each button could look like. You would have to define each buttonA/buttonB/... function yourself.
    u16 buttons = INPUT;
    
    if ((buttons & KEY_A) == KEY_A)
    {
        buttonA();
    }
    if ((buttons & KEY_B) == KEY_B)
    {
        buttonB();
    }
    if ((buttons & KEY_SELECT) == KEY_SELECT)
    {
        buttonSel();
    }
    if ((buttons & KEY_START) == KEY_START)
    {
        buttonS();
    }
    if ((buttons & KEY_RIGHT) == KEY_RIGHT)
    {
        buttonR();
    }
    if ((buttons & KEY_LEFT) == KEY_LEFT)
    {
        buttonL();
    }
    if ((buttons & KEY_UP) == KEY_UP)
    {
        buttonU();
    }
    if ((buttons & KEY_DOWN) == KEY_DOWN)
    {
        buttonD();
    }
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
