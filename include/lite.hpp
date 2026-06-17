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
#include "RenderQueue.hpp"

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

class Data;
class Game;
class TextureManager;

class Map;
class Camera;
class RenderQueue;

class Weapon;
class Projectile;
class ProjectileManager;

class Entity;
class Enemy;
class Player;
class EnemyManager;

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

uint64_t	packCellKey(int32_t x, int32_t y);
int			checkArgs(char* argv[], Data& data);

/**********************************/
// 	---- SDL UTILS FUNCTIONS ---- //
/**********************************/

int				initSDL(Data &data);
int				initSDLText(Data& data);
void			preloadTextures(Data& data);
SDL_Texture*	loadTexture(SDL_Renderer* ren, const std::string& path);

void			drawTextCentered(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy);
void			drawTextLeftAligned(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y);
