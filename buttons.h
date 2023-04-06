/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////
#define PLATFORM_LEFT_BOUND 32 // before hitting left wall
#define PLATFORM_RIGHT_BOUND 208 // before hitting right wall
#define PLATFORM_STEP_SIZE 4 // number of pixels to move at every interrupt
#define PLATFORM_HEIGHT 144 // height of platform

#define BALL_LOWER_BOUND 139 // before hitting platform
#define BALL_UPPER_BOUND 43 // before hitting upper wall

// global variables
int platform_x = 120; // position of platform
int ball_x = 112; // horizontal position of ball
int ball_y = 139; // vertical position of ball
int powerupA_active = 0; // flag to indicate whether the powerup is active or not
int powerupA_timer = 0;
int step_size = PLATFORM_STEP_SIZE;

void buttonA() {
	if (!powerupA_active) {
		powerupA_active = 1;
		step_size *= 2;
		powerupA_timer = 10; // set timer to 10 seconds
	}
}

void timer0Interrupt() {
	if (powerupA_active) {
		powerupA_timer--;
		if (powerupA_timer == 0) {
			step_size /= 2; // revert platform step size
			powerupA_active = 0;
		}
	}
}

void buttonB() {}
void buttonSel() {}
void buttonS() {}
void buttonR() {
    REG_IME = 0x00; // stop all interrupt handling
    if (platform_x <= PLATFORM_RIGHT_BOUND - step_size) {
        platform_x += step_size;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01; // Re-enable interrupt handling
    //if (powerupA_time > 0) { // Check if powerup is active
    //    powerupA_time--; // Decrement powerup time
    //    if (powerupA_time == 0) { // Check if powerup has expired
    //        step_size /= 2; // Restore platform speed to normal
    //    }
    //}
}
void buttonL() {
    REG_IME = 0x00; // stop all interrupt handling
    if (platform_x >= PLATFORM_LEFT_BOUND + step_size) {
        platform_x -= step_size;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01; // Re-enable interrupt handling 
    //if (powerupA_time > 0) {
    //    powerupA_time--;
    //    if (powerupA_time == 0) {
    //        step_size /= 2;
    //    }
    //}
}
void buttonU() {}
void buttonD() {}
