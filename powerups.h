void powerupA_handler() {
    if (powerupA_active) {
        int ones = powerupA_timer % 10;
        int tens = (powerupA_timer / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_POWERUP_A_IND, 56, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_POWERUP_A_IND + 1, 48, 0);
        powerupA_timer--;
        drawSprite(POWERUP_A, POWERUP_IND, 32, 0);
        if (powerupA_timer <= 0) {
            step_size /= 2; // revert platform step size
            powerupA_active = false;
            powerupA_cooldown = POWERUP_A_COOLDOWN_DURATION; // set cooldown
            removeFromScreen(TIMER_POWERUP_A_IND);
            removeFromScreen(TIMER_POWERUP_A_IND+1);
            removeFromScreen(POWERUP_IND);
        }
    } else if (powerupA_cooldown > 0) {
        int ones = powerupA_cooldown % 10;
        int tens = (powerupA_cooldown / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_COOLDOWN_A_IND, 56, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_COOLDOWN_A_IND + 1, 48, 0);
        powerupA_cooldown--;
        if (powerupA_cooldown <= 0) {
            removeFromScreen(TIMER_COOLDOWN_A_IND);
            removeFromScreen(TIMER_COOLDOWN_A_IND+1);
        }
    }
}

void powerupB_handler() {
    if(powerupB_active) {
        int ones = powerupB_timer % 10;
        int tens = (powerupB_timer / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_POWERUP_B_IND, 88, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_POWERUP_B_IND + 1, 80, 0);
        powerupB_timer--;
        drawSprite(POWERUP_B, POWERUPB_IND, 64, 0);
        if (powerupB_timer <= 0) {
            ball_dmg = 1; // revert ball damage to 1
            powerupB_active = false;
            powerupB_cooldown = POWERUP_B_COOLDOWN_DURATION; // set cooldown
            removeFromScreen(POWERUPB_IND);
            removeFromScreen(TIMER_POWERUP_B_IND);
            removeFromScreen(TIMER_POWERUP_B_IND+1);
        }
    } else if (powerupB_cooldown > 0) {
        int ones = powerupB_cooldown % 10;
        int tens = (powerupB_cooldown / 10) % 10;
        drawSprite(NUMBER_ZERO + ones, TIMER_COOLDOWN_B_IND, 88, 0);
        drawSprite(NUMBER_ZERO + tens, TIMER_COOLDOWN_B_IND + 1, 80, 0);
        powerupB_cooldown--;
        if (powerupB_cooldown <= 0) {
            removeFromScreen(TIMER_COOLDOWN_B_IND);
            removeFromScreen(TIMER_COOLDOWN_B_IND+1);
        }
    }
}
