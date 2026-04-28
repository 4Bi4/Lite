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

#include "lite_common.hpp"

/******************************/
//	 ---- CLASS HEADERS ----  //
/******************************/

#include "Data.hpp"

#include "Map.hpp"
#include "Camera.hpp"
#include "TextureManager.hpp"

#include "Entity.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

/******************************/
// --- CLASS DECLARATIONS --- //
/******************************/

class Map;

class Data;
class TextureManager;
class Camera;

class Entity;
class Enemy;
class Player;

/******************************/
// 	---- UTILS FUNCTIONS ---- //
/******************************/

int			checkArgs(char* argv[], Data& data);

/*******************************/
// 	---- RENDER FUNCTIONS ---- //
/*******************************/

void		makeBGRainbow(Data& data);
void		renderBackground(Data& data, SDL_Color color);

/**********************************/
// 	---- SDL UTILS FUNCTIONS ---- //
/**********************************/

int				initSDL(Data &data);
void			drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy);
SDL_Texture*	loadTexture(SDL_Renderer* ren, const std::string& path);