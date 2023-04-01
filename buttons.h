/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////
#define PLATFORM_LEFT_BOUND 32 // before hitting left wall
#define PLATFORM_RIGHT_BOUND 208 // before hitting right wall
#define PLATFORM_STEP_SIZE 4 // pixels to move at every interrupt
#define PLATFORM_HEIGHT 144 // height of platform

#define BALL_LOWER_BOUND 139 // before hitting platform
#define BALL_UPPER_BOUND 43 // before hitting upper wall

// global variables
int platform_x = 120; // position of platform
int ball_x = 112; // horizontal position of ball
int ball_y = 139; // vertical position of ball

void buttonA() {}
void buttonB() {}
void buttonSel() {}
void buttonS() {}
void buttonR() {
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one
    if (platform_x <= PLATFORM_RIGHT_BOUND - PLATFORM_STEP_SIZE) {
        platform_x += PLATFORM_STEP_SIZE;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling
}
void buttonL() {
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one
    if (platform_x >= PLATFORM_LEFT_BOUND + PLATFORM_STEP_SIZE) {
        platform_x -= PLATFORM_STEP_SIZE;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling
}
void buttonU() {}
void buttonD() {}