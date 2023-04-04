/////////////////////////////////////////////////
// functions that handles what each button does
/////////////////////////////////////////////////
#define PLATFORM_LEFT_BOUND 32 // before hitting left wall
#define PLATFORM_RIGHT_BOUND 208 // before hitting right wall
#define PLATFORM_STEP_SIZE 4 // pixels to move platform at every interrupt
#define PLATFORM_HEIGHT 144 // height of platform

#define BALL_UPPER_BOUND 32 // before hitting upper wall
#define BALL_LEFT_BOUND 16 // before hitting left wall
#define BALL_RIGHT_BOUND 224 // before hitting right wall
#define BALL_STEP_SIZE 2 // pixels to move ball at every interrupt
#define BALL_RADIUS 3 // ball radius

#define GAME_START_COUNTDOWN 3 // countdown length before starting game
#define GAME_STARTING 0 // game_state for starting game
#define GAME_STARTED 1 // game_state for game started
#define GAME_PAUSED 2 // game_state for game paused

#define M_PI 3.14159265358979323846  // pi

// global variables
int platform_x = 120; // position of platform
int ball_x = 112; // horizontal position of ball
int ball_y = 139; // vertical position of ball
double ball_heading = -M_PI/2; // heading for ball movement [-pi,pi] increase clockwise
int timer = 0; // overall timer
int num_life = 4; // number of life left
int game_state = GAME_PAUSED; // track status of game
/*
  0: game starting
  1: game started
  2: game paused
*/

void buttonA() {}
void buttonB() {}
void buttonSel() {}
void buttonS() {}
void buttonR() {
    if (platform_x <= PLATFORM_RIGHT_BOUND - PLATFORM_STEP_SIZE) {
        platform_x += PLATFORM_STEP_SIZE;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
        if (game_state == GAME_STARTING) {
            ball_x += PLATFORM_STEP_SIZE;
            drawSprite(BALL, BALL_IND, ball_x, ball_y);
        }
    }
}
void buttonL() {
    if (platform_x >= PLATFORM_LEFT_BOUND + PLATFORM_STEP_SIZE) {
        platform_x -= PLATFORM_STEP_SIZE;
        drawSprite(PLATFORM_LEFT, PLATFORM_LEFT_IND, platform_x-16, PLATFORM_HEIGHT);
        drawSprite(PLATFORM_RIGHT, PLATFORM_RIGHT_IND, platform_x, PLATFORM_HEIGHT);
        if (game_state == GAME_STARTING) {
            ball_x -= PLATFORM_STEP_SIZE;
            drawSprite(BALL, BALL_IND, ball_x, ball_y);
        }
    }
}
void buttonU() {}
void buttonD() {}