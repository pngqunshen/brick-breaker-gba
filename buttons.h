/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////

#define INPUT                      (KEY_MASK & (~REG_KEYS))

void buttonA() {
    switch (game_state)
    {
    case GAME_STARTED: {
        if (powerupA_cooldown > 0) {
            // Powerup is on cooldown, do nothing
            break;
        }
        if (!powerupA_active) {
            powerupA_active = true;
            step_size *= 2;
            powerupA_timer = POWERUP_A_DURATION; // set powerup timer
        }
        break;
    }

    case GAME_OVER: {
        initialise();
        game_state = GAME_MENU;
        break;
    }
    
    default:
        break;
    }
}

void buttonB() {}
void buttonSel() {}
void buttonS() {
    switch (game_state)
    {
    case GAME_PAUSED: {
        if (pause_timer <= 0) {
            game_state = GAME_STARTING;
            pause_timer = GAME_PAUSE_COOLDOWN;
            start_timer = GAME_START_COUNTDOWN;
        }
        break;
    }

    case GAME_STARTING:
    case GAME_STARTED: {
        if (pause_timer <= 0) {
            pause_timer = GAME_PAUSE_COOLDOWN;
            game_state = GAME_PAUSED;
        }
        break;
    }

    case GAME_MENU: {
        int i;
        for (i = 0; i<60; i++) {
            drawSprite(0,i,240,160);
        }
        game_state = GAME_STARTING;
        initialiseLevelOne();
        break;
    }

    case GAME_OVER: {
        initialise();
        game_state = GAME_MENU;
        break;
    }
    
    default:
        break;
    }
}

void buttonR() {
    switch (game_state)
    {
    case GAME_STARTED:
    case GAME_ENDING: {
        if (platform_x <= PLATFORM_RIGHT_BOUND - step_size) {
            platform_x += step_size;
            drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_Y);
            drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_Y);
            if (game_state == GAME_STARTING) {
                ball_x += PLATFORM_STEP_SIZE;
                drawSprite(BALL, BALL_IND, ball_x, ball_y);
            }
        }
        break;
    }
    
    default:
        break;
    }
}
void buttonL() {
    switch (game_state)
    {
    case GAME_STARTED:
    case GAME_ENDING: {
        if (platform_x >= PLATFORM_LEFT_BOUND + step_size) {
            platform_x -= step_size;
            drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_Y);
            drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_Y);
            if (game_state == GAME_STARTING) {
                ball_x -= PLATFORM_STEP_SIZE;
                drawSprite(BALL, BALL_IND, ball_x, ball_y);
            }
        }
        break;
    }
    
    default:
        break;
    }
}
void buttonU() {}
void buttonD() {}

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
