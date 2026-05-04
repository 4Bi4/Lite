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

#include "Camera.hpp"
#include "Map.hpp"
#include "TextureManager.hpp"

#include "Entity.hpp"
#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "EnemyManager.hpp"

#include "Projectile.hpp"
#include "ProjectileManager.hpp"

#include "Game.hpp"

#include "Data.hpp"

/******************************/
// --- CLASS DECLARATIONS --- //
/******************************/

class Map;

class Data;
class TextureManager;
class Camera;

class Entity;
class Weapon;
class Enemy;
class Player;
class EnemyManager;
class Game;

/*****************************/
// 	---- GAME FUNCTIONS ---- //
/*****************************/

int			handleEvents(Data& data, SDL_Event& event);

/*******************************/
// 	---- RENDER FUNCTIONS ---- //
/*******************************/

void		makeBGRainbow(Data& data);
void		renderBackground(Data& data, SDL_Color color);

/******************************/
// 	---- UTILS FUNCTIONS ---- //
/******************************/

int			checkArgs(char* argv[], Data& data);

/**********************************/
// 	---- SDL UTILS FUNCTIONS ---- //
/**********************************/

int				initSDL(Data &data);
int				initSDLText(Data& data);
void			loadTextures(Data& data);
void			drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy);
SDL_Texture*	loadTexture(SDL_Renderer* ren, const std::string& path);