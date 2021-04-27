#ifndef BASE_
#define BASE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define TIME_MAX 1000
#define GROUND 430
#define MAX_HEIGHT 250
#define BASE_OFFSET_SPEED 0
#define SPEED_INCREASEMENT 2
#define SCORE_INCREASEMENT 1
#define TIME_INCREASEMENT 1
#define FRAME_INCREASEMENT 1
#define GROUND_SPEED 5
#define ENEMY_SPEED 5
#define MAX_ENEMY_WIDTH 100

#define IN_AIR_ENEMY 1
#define ON_GROUND_ENEMY 0

#define SLOW_FRAME_CHAR 4
#define SLOW_FRAME_ENEMY 2

#define REPEATITIVE -1
#define NOT_REPEATITIVE 0

#define SMALL_BUTTON 1
#define COMMON_BUTTON 2

const std::string WINDOW_TITLE = "Monkey D.Luffy";

const int SCREEN_WIDTH = 928;
const int SCREEN_HEIGHT = 522;

const int COMMON_BUTTON_WIDTH = 150;
const int COMMON_BUTTON_HEIGHT = 98;
const int SMALL_BUTTON_WIDTH = 22;
const int SMALL_BUTTON_HEIGHT = 34;

const int PLAY_BUTON_POSX = 389;
const int PLAY_BUTTON_POSY = 186;
const int HELP_BUTTON_POSX = 389;
const int HELP_BUTTON_POSY = 293;
const int EXIT_BUTTON_POSX = 389;
const int EXIT_BUTTON_POSY = 402;
const int BACK_BUTTON_POSX = 31;
const int BACK_BUTTON_POSY = 29;
const int PAUSE_BUTTON_POSX = 31;
const int PAUSE_BUTTON_POSY = 29;
const int CONTINUE_BUTTON_POSX = 31;
const int CONTINUE_BUTTON_POSY = 29;

const int TEXT_1_POSX = 700;
const int TEXT_1_POSY = 20;
const int TEXT_2_POSX = 700;
const int TEXT_2_POSY = 80;
const int SCORE_POSX = 830;
const int SCORE_POSY = 20;
const int HIGH_SCORE_POSX = 830;
const int HIGH_SCORE_POSY = 80;

const double LAYER_1_SPEED = 2;
const double LAYER_2_SPEED = 0.5;

const int RUNNING_FRAMES = 8;
const int FLYING_FRAMES = 4;
const int BACKGROUND_LAYER = 2;

enum ButtonSprite
{
	BUTTON_MOUSE_OUT = 0,
	BUTTON_MOUSE_OVER = 1,
	BUTTON_TOTAL = 2
};

#endif 