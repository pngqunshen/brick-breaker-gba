void initialise(void)
{
	// game coordinates
	platform_x = 120; // position of platform
	ball_x = BALL_START_X; // horizontal position of ball
	ball_y = BALL_START_Y; // vertical position of ball
	ball_heading = BALL_START_HEAD; // heading for ball movement [-pi,pi) increase clockwise

	// powerups
	powerupA_active = false; // flag to indicate whether the powerup is active or not
	powerupA_timer = 0; // timer for powerupA duration
	powerupA_cooldown = 0; // timer for powerupA cooldown
	step_size = PLATFORM_STEP_SIZE; // step size to move platform
	powerupB_active = false; // flag to indicate whether the powerup is active or not
	powerupB_timer = 0; // timer for powerupA duration
	powerupB_cooldown = 0; // timer for powerupA cooldown
	ball_dmg = BALL_DAMAGE; // step size to move platform

	// timer
	timer = GAME_DURATION; // overall timer
	pause_timer = GAME_PAUSE_COOLDOWN; // time taken before game can unpause
	start_timer = GAME_START_COUNTDOWN; // time taken before game starts
	next_level_timer = NEXT_LEVEL_COUNTDOWN; // time taken before next level kicks in

	// other game states
	num_life = MAX_NUM_LIFE; // number of life left
	game_state = GAME_MENU; // track status of game
	main_menu_flash = true; // flash the press start message
	bricks_eliminated = 0; // counter for number of bricks eliminated
	current_level = 1; // current level of game

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
	drawHeart();
}

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

	switch (game_state)
	{
	case GAME_MENU: {
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
			int i;
			for (i=12; i<22; i++) {
            	removeFromScreen(START_GAME_MESSAGE_IND+i);
			}
		}
		break;
	}

	case GAME_MENU_LEVEL: {
		drawSprite(LETTER_L, START_GAME_MESSAGE_IND+12, 92, 92);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+13, 100, 92);
		drawSprite(LETTER_V, START_GAME_MESSAGE_IND+14, 108, 92);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+15, 116, 92);
		drawSprite(LETTER_L, START_GAME_MESSAGE_IND+16, 124, 92);
		drawSprite(NUMBER_ONE, START_GAME_MESSAGE_IND+17, 140, 92);

		drawSprite(LETTER_L, START_GAME_MESSAGE_IND+18, 92, 108);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+19, 100, 108);
		drawSprite(LETTER_V, START_GAME_MESSAGE_IND+20, 108, 108);
		drawSprite(LETTER_E, START_GAME_MESSAGE_IND+21, 116, 108);
		drawSprite(LETTER_L, START_GAME_MESSAGE_IND+22, 124, 108);
		drawSprite(NUMBER_TWO, START_GAME_MESSAGE_IND+23, 140, 108);
		
		switch (current_level)
		{
		case 1: {
			if (main_menu_flash) {
				drawSprite(BALL, START_GAME_MESSAGE_IND+24, 148, 92);
			} else {
				removeFromScreen(START_GAME_MESSAGE_IND+24);
			}
			break;
		}

		case 2: {
			if (main_menu_flash) {
				drawSprite(BALL, START_GAME_MESSAGE_IND+24, 148, 108);
			} else {
				removeFromScreen(START_GAME_MESSAGE_IND+24);
			}
			break;
		}
		
		default:
			break;
		}
		break;
	}
	
	default:
		break;
	}
}

void gameOver(void)
{
	drawSprite(LETTER_G, GAME_MESSAGE_IND, 88, 72);
	drawSprite(LETTER_A, GAME_MESSAGE_IND+1, 96, 72);
	drawSprite(LETTER_M, GAME_MESSAGE_IND+2, 104, 72);
	drawSprite(LETTER_E, GAME_MESSAGE_IND+3, 112, 72);
	drawSprite(LETTER_O, GAME_MESSAGE_IND+4, 120, 72);
	drawSprite(LETTER_V, GAME_MESSAGE_IND+5, 128, 72);
	drawSprite(LETTER_E, GAME_MESSAGE_IND+6, 136, 72);
	drawSprite(LETTER_R, GAME_MESSAGE_IND+7, 144, 72);

	if (main_menu_flash) {
		drawSprite(LETTER_T, START_GAME_MESSAGE_IND, 84, 104);
		drawSprite(LETTER_R, START_GAME_MESSAGE_IND+1, 92, 104);
		drawSprite(LETTER_Y, START_GAME_MESSAGE_IND+2, 100, 104);
		drawSprite(LETTER_A, START_GAME_MESSAGE_IND+3, 116, 104);
		drawSprite(LETTER_G, START_GAME_MESSAGE_IND+4, 124, 104);
		drawSprite(LETTER_A, START_GAME_MESSAGE_IND+5, 132, 104);
		drawSprite(LETTER_I, START_GAME_MESSAGE_IND+6, 140, 104);
		drawSprite(LETTER_N, START_GAME_MESSAGE_IND+7, 148, 104);
	} else {
		int i;
		for (i=0; i<8; i++) {
			removeFromScreen(START_GAME_MESSAGE_IND+i);
		}
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
		removeFromScreen(BRICKS_IND + i);
	}
	bricks_eliminated = 0;
	for (i = 0; i < 27; i++) {
		int xb = 16*(i%9) + 48; // restart row after 9 bricks
		int yb = 48 + (i/9)*8; // go second row after 9 bricks
		bricks[i][0] = xb+8;
		bricks[i][1] = yb+4;
		drawBrick(xb, yb, i);
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
		removeFromScreen(BRICKS_IND + i);
	}
	bricks_eliminated = 0;
	for (i = 0; i < 45; i++) {
		int xb = 16*(i%9) + 48; // restart row after 9 bricks
		int yb = 32 + (i/9)*8; // go second row after 9 bricks
		bricks[i][0] = xb+8;
		bricks[i][1] = yb+4;
		drawBrick(xb, yb, i);
	}
	game_state = GAME_STARTING; // unpause game, begin countdown
}

