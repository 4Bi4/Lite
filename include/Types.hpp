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

//	MAIN DATA STUFF

//	Holds all the SDL-related data
struct SdlData
{
	SDL_Window*		window = nullptr;
	SDL_Renderer*	renderer = nullptr;
	TTF_Font*		fontLarge = nullptr;
	TTF_Font*		fontSmall = nullptr;
};

//	State machine
enum state
{
	LOADING,
	MAIN_MENU,
	IN_GAME,
	POSTGAME,
	CLOSING,
};

//	ENTITY STUFF

struct EntityID
{
	int32_t index = -1;
	uint32_t gen = 0;

	bool isValid() const;
	bool operator==(const EntityID& other);
};

inline bool EntityID::isValid() const
{
	return (index != -1);
}

inline bool EntityID::operator==(const EntityID& other)
{
	return ((this->index == other.index) && (this->gen == other.gen));
}

//	WEAPON STUFF

enum WeaponType
{
	FIREBALL
};

//	PROJECTILE STUFF

struct ProjectileID
{
	int32_t index = -1;
	uint32_t gen = 0;

	bool isValid() const;
	bool operator==(const ProjectileID& other);
};

inline bool ProjectileID::isValid() const
{
	return (index != -1);
}

inline bool ProjectileID::operator==(const ProjectileID& other)
{
	return ((this->index == other.index) && (this->gen == other.gen));
}

struct ProjectileStats
{
	int		damage = 0;
	float	speed = 0.0f;
	int		pierceCount = 0;
	float	knockback = 0.0f;
	float	sizeMultiplier = 1.0f;
};

enum ProjectileType
{
	FIRE,
	BULLET_SMALL
};
