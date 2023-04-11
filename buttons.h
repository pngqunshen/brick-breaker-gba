/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////

#define INPUT                      (KEY_MASK & (~REG_KEYS))

void powerupA_handler() {
	if (powerupA_active) {
		powerupA_timer--;
		if (powerupA_timer == 0) {
			step_size /= 2; // revert platform step size
			powerupA_active = 0;
		}
	}
}

void buttonA() {
	if (!powerupA_active) {
		powerupA_active = 1;
		step_size *= 2;
		powerupA_timer = 10; // set timer to 10 seconds
	}
}
void buttonB() {}
void buttonSel() {}
void buttonS() {
    if (game_state == GAME_PAUSED && pause_timer <= 0) {
        game_state = GAME_STARTING;
        pause_timer = GAME_PAUSE_COOLDOWN;
        start_timer = GAME_START_COUNTDOWN;
    } else if ((game_state == GAME_STARTING || game_state == GAME_STARTED) && pause_timer <= 0) {
        pause_timer = GAME_PAUSE_COOLDOWN;
        game_state = GAME_PAUSED;
    }
}
void buttonR() {
    if (game_state == GAME_STARTED || game_state == GAME_ENDING) {
        if (platform_x <= PLATFORM_RIGHT_BOUND - step_size) {
            platform_x += step_size;
            drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
            drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
            if (game_state == GAME_STARTING) {
                ball_x += PLATFORM_STEP_SIZE;
                drawSprite(BALL, BALL_IND, ball_x, ball_y);
            }
        }
        //if (powerupA_time > 0) { // Check if powerup is active
        //    powerupA_time--; // Decrement powerup time
        //    if (powerupA_time == 0) { // Check if powerup has expired
        //        step_size /= 2; // Restore platform speed to normal
        //    }
        //}
    }
}
void buttonL() {
    if (game_state == GAME_STARTED || game_state == GAME_ENDING) {
        if (platform_x >= PLATFORM_LEFT_BOUND + step_size) {
            platform_x -= step_size;
            drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
            drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
            if (game_state == GAME_STARTING) {
                ball_x -= PLATFORM_STEP_SIZE;
                drawSprite(BALL, BALL_IND, ball_x, ball_y);
            }
        }
        //if (powerupA_time > 0) {
        //    powerupA_time--;
        //    if (powerupA_time == 0) {
        //        step_size /= 2;
        //    }
        //}
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
