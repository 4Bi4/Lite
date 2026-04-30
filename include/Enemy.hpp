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

//	Here we will list the type of enemies so
//	they can be easily identified
enum enemyType
{
	DEFAULT
};

class EnemyManager;

class Enemy : public Entity
{
public:
	Enemy(SDL_Texture* texture, EnemyManager* manager);
	~Enemy();

	void	calcNextMove(Data& data);
	void	update(float deltaTime, Data& data);
	void	render(Data& data);

	const SDL_FRect&	getRect() const;
	enemyType			getType() const;
	const SDL_Texture*	getTexture() const;
	int					getHp() const;
	int					getMaxHp() const;

	void	setType(enemyType type);
	void	setTexture(SDL_Texture* texture);
	void	setPosition(float x, float y);
	void	takeDamage(int damage);
	void	heal(int amount);
	void	setMaxHp(int hp);


protected:
	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;		// The crop of the original image to render (x,y,w,h)

	EnemyManager*	_manager;

	enemyType		_type;

	int				_hp;
	int				_maxHp;
	float			_speed;
	float			_dirX;
	float			_dirY;
};
