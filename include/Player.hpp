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

class Game;

class Player : public Entity
{
public:
	Player(SDL_Texture* texture);
	~Player();

	void	handleInput(Data& data);
	void	update(float deltaTime, Data& data);
	void	addToQueue(Data& data);

	void	attack(Game& game) const;

	const SDL_FRect&	getRect() const;
	const SDL_Texture*	getTexture() const;
	EntityID			getTarget() const;
	Weapon*				getWeapon() const;
	EntityID			getClosestEnemy(EnemyManager& enemyManager) const;
	int					getHp() const;
	int					getMaxHp() const;
	bool				isInvulnerable() const;

	void	setPosition(float x, float y);
	void	setTexture(SDL_Texture* texture);
	void	setTarget(EntityID enemy, Game& game);
	void	setWeapon(Weapon* weapon);
	void	takeDamage(int damage);
	void	heal(int amount);
	void	setMaxHp(int hp);
	void 	setInvulnerable(int invulnerable);
	void 	setInvulnerable();

protected:
	void	handleKeyboardMovement();
	void	handleGamepadMovement(Data& data);
	void	updateInvulnerabilityTimer();

	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;	

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;		// The crop of the original image to render (x,y,w,h)

	EntityID		_target;		//	Pointer to the enemy we're currently targeting (nullptr if none)
	Weapon*			_weapon;		//	Pointer to the player's weapon (nullptr if none)

	int				_hp;
	int				_maxHp;
	Uint64			_invulnerabilityTimer;
	bool			_isInvulnerable;
	Uint64			_lastDamageTime;
	float			_speed;
	float			_dirX;
	float			_dirY;
};
