void mainMenu(void) {
	drawSprite(LETTER_B, START_GAME_MESSAGE_IND, 68, 60);
	drawSprite(LETTER_R, START_GAME_MESSAGE_IND+1, 76, 60);
	drawSprite(LETTER_I, START_GAME_MESSAGE_IND+2, 84, 60);
	drawSprite(LETTER_C, START_GAME_MESSAGE_IND+3, 92, 60);
	drawSprite(LETTER_K, START_GAME_MESSAGE_IND+4, 100, 60);
	drawSprite(LETTER_B, START_GAME_MESSAGE_IND+5, 116, 60);
	drawSprite(LETTER_R, START_GAME_MESSAGE_IND+6, 124, 60);
	drawSprite(LETTER_E, START_GAME_MESSAGE_IND+7, 132, 60);
	drawSprite(LETTER_A, START_GAME_MESSAGE_IND+8, 140, 60);
	drawSprite(LETTER_K, START_GAME_MESSAGE_IND+9, 148, 60);
	drawSprite(LETTER_E, START_GAME_MESSAGE_IND+10, 156, 60);
	drawSprite(LETTER_R, START_GAME_MESSAGE_IND+11, 164, 60);

	if (main_menu_flash) {
		drawSprite(LETTER_P, START_GAME_MESSAGE_IND+12, 76, 92);
		drawSprite(LETTER_R, START_GAME_MESSAGE_IND+13, 84, 92);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+14, 92, 92);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+15, 100, 92);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+16, 108, 92);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+17, 124, 92);
		drawSprite(LETTER_T, START_GAME_MESSAGE_IND+18, 132, 92);
		drawSprite(LETTER_A, START_GAME_MESSAGE_IND+19, 140, 92);
		drawSprite(LETTER_R, START_GAME_MESSAGE_IND+20, 148, 92);
		drawSprite(LETTER_T, START_GAME_MESSAGE_IND+21, 156, 92);
	} else {
		drawSprite(LETTER_P, START_GAME_MESSAGE_IND+12, 240, 160);
		drawSprite(LETTER_R, START_GAME_MESSAGE_IND+13, 240, 160);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+14, 240, 160);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+15, 240, 160);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+16, 240, 160);
		drawSprite(LETTER_S, START_GAME_MESSAGE_IND+17, 240, 160);
		drawSprite(LETTER_T, START_GAME_MESSAGE_IND+18, 240, 160);
		drawSprite(LETTER_A, START_GAME_MESSAGE_IND+19, 240, 160);
		drawSprite(LETTER_R, START_GAME_MESSAGE_IND+20, 240, 160);
		drawSprite(LETTER_T, START_GAME_MESSAGE_IND+21, 240, 160);
	}
}

void initialiseLevelOne(void) 
{
	int i; // general loop variable
	// platform
	drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
	drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
	// ball
	drawSprite(BALL, BALL_IND, ball_x, ball_y);
	// brick
	for (i = 0; i < BRICK_MAX_NUM; i++) {
		bricks[i][0] = -1;
		bricks[i][1] = -1;
		brick_health[i] = BRICK_MAX_HEALTH;
		drawSprite(BRICK_RED, BRICKS_IND + i, 240, 160);
	}
	for (i = 0; i < 27; i++) {
		int xb = 16*(i%9) + 48; // restart row after 9 bricks
		int yb = 32 + (i/9)*8; // go second row after 9 bricks
		bricks[i][0] = xb+8;
		bricks[i][1] = yb+4;
		drawSprite(BRICK_RED, BRICKS_IND + i, xb, yb);
	}
	game_state = GAME_STARTING; // unpause game, begin countdown
}

void initialiseLevelTwo(void) 
{
	int i; // general loop variable
	// platform
	drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
	drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
	// ball
	drawSprite(BALL, BALL_IND, ball_x, ball_y);
	// brick
	for (i = 0; i < BRICK_MAX_NUM; i++) {
		bricks[i][0] = -1;
		bricks[i][1] = -1;
		brick_health[i] = BRICK_MAX_HEALTH;
		drawSprite(BRICK_RED, BRICKS_IND + i, 240, 160);
	}
	for (i = 0; i < 45; i++) {
		int xb = 16*(i%9) + 48; // restart row after 9 bricks
		int yb = 32 + (i/9)*8; // go second row after 9 bricks
		bricks[i][0] = xb+8;
		bricks[i][1] = yb+4;
		drawSprite(BRICK_RED, BRICKS_IND + i, xb, yb);
	}
	game_state = GAME_STARTING; // unpause game, begin countdown
}

