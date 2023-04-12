void initialise_level_one(void) 
{
	int i; // general loop variable
	// platform
	drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, 144);
	drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, 144);
	// ball
	drawSprite(BALL, BALL_IND, ball_x, ball_y);
	// brick
	for (i = 0; i < 11; i++) {
		drawSprite(BRICK_RED, BRICKS_IND + i, 16*i + 32, 32);
		drawSprite(BRICK_RED, BRICKS_IND + 11 + i, 16*i + 32, 40);
		// int brick_hp[2][10] = {(1, 1, 1, 1, 1, 1, 1, 1, 1, 1),(1, 1, 1, 1, 1, 1, 1, 1, 1, 1)};
		// int hori_edges[4] = {32, 39, 40, 47};
		// int verti_edges[20] = {32, 47, 48, 63, 64, 79, 80, 95, 96, 111, 112, 127, 128, 143, 144, 159, 160, 175, 176, 191};
	}
	game_state = GAME_STARTING; // unpause game, begin countdown
}
