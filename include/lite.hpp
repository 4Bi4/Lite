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

#include <string>
#include <iostream>
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_image/SDL_image.h"

//	COLOR DEFINITIONS

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

/******************************/
//	---- DEFAULT VALUES ----  //
/******************************/

//	Default  horizontal resolution
#define DEFAULT_HRES 1280
//	Default vertical resolution
#define DEFAULT_VRES 720

//	Default FPS limit
#define DEFAULT_FPS_LIMIT 60

//	Global variable for debug mode
//	Debug Mode prints information on the console about the engine's state
namespace Debug
{
	inline bool state = false;
}

/******************************/
//	 ---- CLASS HEADERS ----  //
/******************************/

#include "Data.hpp"

/******************************/
// 	---- UTILS FUNCTIONS ---- //
/******************************/

int				checkArgs(char* argv[]);

/**********************************/
// 	---- SDL UTILS FUNCTIONS ---- //
/**********************************/

int				initSDL(Data &data);
void			drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy);
SDL_Texture*	loadTexture(SDL_Renderer* ren, const std::string& path);