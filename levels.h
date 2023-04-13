void initialise_level_one(void) 
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
	}
	for (i = 0; i < 11; i++) {
		drawSprite(BRICK_RED, BRICKS_IND + i, 16*i + 32, 32);
		drawSprite(BRICK_RED, BRICKS_IND + 11 + i, 16*i + 32, 40);
		bricks[i*2][0] = 16*i + 40;
		bricks[i*2][1] = 36;
		bricks[i*2+1][0] = 16*i + 40;
		bricks[i*2+1][1] = 44;
	}
	game_state = GAME_STARTING; // unpause game, begin countdown
}
