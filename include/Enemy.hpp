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

class EnemyManager;

class Enemy : public Entity
{
public:
	Enemy(SDL_Texture* texture, EnemyManager* manager);
	~Enemy();

	void	setUp(EnemyType type, SDL_Texture* texture);

	void	spawn(EntityID id, float x, float y);
	void	die();

	void	calcNextMove(Game& game);
	void	addToQueue(Data& data);
	void	update(float deltaTime, Game& game);

	int					getHp() const;
	EntityID			getID() const;
	const SDL_FRect&	getRect() const;
	const SDL_FRect&	getHitbox() const;
	EnemyType			getType() const;
	bool				isActive() const;
	int					getMaxHp() const;
	const SDL_Texture*	getTexture() const;

	void	heal(int amount);
	void	setMaxHp(int hp);
	void	takeDamage(int damage);
	void	setType(EnemyType type);
	void	setPosition(float x, float y);
	void	setTexture(SDL_Texture* texture);

protected:
	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;
	SDL_FRect		_hitbox;		// The hitbox for collision detection (x,y,w,h)

	EnemyManager*	_manager;

	EnemyType		_type;
	EntityID		_id;

	bool			_active;
	int				_hp;
	int				_maxHp;
	int				_damage;
	float			_speed;
	float			_dirX;
	float			_dirY;
};
