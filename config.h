// define various index of sprites in spriteMem
#define BALL_IND 0 // first ball index
#define PLATFORM_LEFT_IND 5 // left platform index
#define PLATFORM_RIGHT_IND 6 // right platform index
#define BRICKS_IND 10 // first brick index
#define LIFE_IND 60 // first life index
#define POWERUP_IND 65 // first powerup index
#define LEFT_WALL_IND 70 // first left wall index
#define RIGHT_WALL_IND 80 // first right wall index
#define TOP_WALL_IND 90 // first right wall index
#define TIMER_OVERALL_IND 110 // first overall timer index
#define TIMER_START_IND 113 // game start timer index
#define TIMER_POWERUP_IND 114 // powerup timer index
#define TIMER_COOLDOWN_IND 116 // cooldown timer index
#define GAME_MESSAGE_IND 118 // game message index
#define START_GAME_MESSAGE_IND 0 // start game message index, overwrites ball index

// platform config
#define PLATFORM_LEFT_BOUND 32 // before hitting left wall
#define PLATFORM_RIGHT_BOUND 208 // before hitting right wall
#define PLATFORM_STEP_SIZE 4 // pixels to move platform at every interrupt
#define PLATFORM_HEIGHT 144 // height of platform
#define PLATFORM_MAX_ANGLE M_PI/6 // maximum additional deflection by hitting end of platform

// ball config
#define BALL_UPPER_BOUND 32 // before hitting upper wall
#define BALL_LEFT_BOUND 16 // before hitting left wall
#define BALL_RIGHT_BOUND 223 // before hitting right wall
#define BALL_PLATFORM_BOUND 149 // before hitting platform
#define BALL_STEP_SIZE 4 // pixels to move ball at every interrupt
#define BALL_RADIUS 3 // ball radius
#define BALL_START_X 112 // start x position of ball
#define BALL_START_Y 90 // start y position of ball
#define BALL_START_HEAD M_PI/2 // start heading of ball

// powerup config
#define POWERUP_A_DURATION 10 // duration of powerup A
#define POWERUP_A_COOLDOWN_DURATION 30 // cooldown time for powerup A

// brick config
#define BRICK_MAX_NUM 50 // maximum number of bricks supported
#define BRICK_LENGTH 16 // brick length
#define BRICK_HEIGHT 8 // brick height
#define BRICK_THRESHOLD 2 // brick pixel threshold such that hit is still registered
#define BRICK_MAX_HEALTH 1 // brick max health

// game config
#define GAME_DURATION 300 // length of game in seconds
#define GAME_START_COUNTDOWN 3 // countdown length before starting game
#define GAME_PAUSE_COOLDOWN 2 // cooldown time when pause initiated
#define MAX_NUM_LIFE 4 // cooldown time when pause initiated

enum GameState
{
    /* data */
    GAME_MENU, // game_state for start menu
    GAME_STARTED, // game_state for game started
    GAME_STARTING, // game_state for starting game
    GAME_PAUSED, // game_state for game paused
    GAME_ENDING, // game_state for game ending
    GAME_ENDED, // game_state for game ended
    GAME_OVER // game_state for game over
};


#define M_PI 3.14159265358979323846  // pi

/////////////////////////////////////
// global variables
/////////////////////////////////////

// game coordinates
int platform_x = 120; // position of platform
int ball_x = BALL_START_X; // horizontal position of ball
int ball_y = BALL_START_Y; // vertical position of ball
double ball_heading = BALL_START_HEAD; // heading for ball movement [-pi,pi) increase clockwise

// powerups
bool powerupA_active = false; // flag to indicate whether the powerup is active or not
int powerupA_timer = 0; // timer for powerupA duration
int powerupA_cooldown = 0; // timer for powerupA cooldown
int step_size = PLATFORM_STEP_SIZE; // step size to move platform

// timer
int timer = GAME_DURATION; // overall timer
int pause_timer = GAME_PAUSE_COOLDOWN; // time taken before game can unpause
int start_timer = GAME_START_COUNTDOWN; // time taken before game starts

// other game states
int num_life = MAX_NUM_LIFE; // number of life left
enum GameState game_state = GAME_MENU; // track status of game
bool main_menu_flash = true; // flash the press start message

// bricks
int bricks[BRICK_MAX_NUM][2]; // position of brick
int brick_health[BRICK_MAX_NUM]; // health of brick
