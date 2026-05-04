/***************************************************************\
*                                                               *
*   ▓██▓    ▓██▓▄▄▄█████▓▓█████                                 *
*   ▓██▒    ▓██▒▓  ██▒ ▓▒▓█   ▀           Lite Engine           *
*   ▒██░    ▒██▒▒ ▓██░ ▒░▒███                                   *
*   ▒██░    ░██░░ ▓██▓ ░ ▒█   ▄            written by:          *
*   ░██████▒░██░  ▒██▒ ░ ░▒████▒       4Bi4 aka labia-fe        *
*   ░ ▒░▓  ░░▓    ▒ ░░   ░░ ▒░ ░              and               *
*   ░ ░ ▒  ░ ▒ ░    ░     ░ ░  ░          monocleduck           *
*     ░ ░    ▒ ░  ░         ░                                   *
*       ░  ░ ░              ░  ░      created: 07/04/2026       *
*                                                               *
\***************************************************************/

#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <atomic>
#include <cstdlib>
#include <iomanip>
#include <csignal>
#include <iostream>
#include <unordered_map>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_image/SDL_image.h"

//	Homemade definitions

#include "Types.hpp"

//	Global COLOR DEFINITIONS

#define RED			"\033[0;31m"
#define GREEN		"\033[0;32m"
#define YELLOW		"\033[0;33m"
#define BLUE		"\033[0;34m"
#define MAGENTA		"\033[0;35m"
#define CYAN		"\033[0;36m"

#define B_RED		"\033[1;31m"
#define B_GREEN		"\033[1;32m"
#define B_YELLOW	"\033[1;33m"
#define B_BLUE		"\033[1;34m"
#define B_MAGENTA	"\033[1;35m"
#define B_CYAN		"\033[1;36m"

#define NO_COLOR	"\033[0m"

/****************************/
//---- DEFAULT VALUES ----  //
/****************************/

//	Max float
#define MAXFLOAT	3.40282347e+38F

//	Default resolution
#define DEFAULT_HRES 1280
#define DEFAULT_VRES 720

#define VOID_TILES 3	//	Number of tiles from the edge of the map we want to render
#define TARGET_HEIGHT 1080.0f
#define TARGET_WIDTH 1920.0f

//	This is more like a zoom adjustment tbh
#define FOV 20.0f

//	Default FPS limit
#define DEFAULT_FPS_LIMIT 144

//	Tile size in pixels
#define PIXEL_SIZE 64

//	Map size in tiles
#define DEFAULT_MAP_WIDTH 35
#define DEFAULT_MAP_HEIGHT 25

//	Default texture paths
#define DIRT_TEXTURE "./resources/textures/map/wall.png"
#define GRASS_TEXTURE "./resources/textures/map/grass.png"

#define DEBUG_ENEMY_TEXTURE "./resources/textures/enemy/targeted_enemy.png"
#define DEFAULT_ENEMY_TEXTURE "./resources/textures/enemy/enemy.png"
#define DEFAULT_PLAYER_TEXTURE "./resources/textures/player/error.png"

#define FIREBALL_TEXTURE "./resources/textures/fireball.png"

//	Gamepad management
#define MAX_JOYSTICK_VALUE 32767.0f
#define DEADZONE 8000

/****************************/
//---- GAME CONSTANTS ----  //
/****************************/

#define ROUND_TIME 20.0f	// In seconds

#define MAX_ENEMIES 2000
#define MAX_PROJECTILES 2000

//	Global variable for debug mode
//	Debug Mode prints information on the console about the engine's state
namespace Debug
{
	inline bool state = false;
}

class Data;
