/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////

// global variables
int platform_x = 120;

void buttonA() {}
void buttonB() {}
void buttonSel() {}
void buttonS() {}
void buttonR() {
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one
    if (platform_x < 208) {
        platform_x += 4;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling
}
void buttonL() {
    REG_IME = 0x00; // Stop all other interrupt handling, while we handle this current one
    if (platform_x > 32) {
        platform_x -= 4;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
    }
    REG_IF = REG_IF; // Update interrupt table, to confirm we have handled this interrupt
    REG_IME = 0x01;  // Re-enable interrupt handling
}
void buttonU() {}
void buttonD() {}