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
