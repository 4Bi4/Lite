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

class Data;

class Entity
{
public:
	Entity(SDL_Texture* texture);
	virtual ~Entity() = 0;

	virtual void		update(float deltaTime, Data& data);
	virtual void		render(Data& data);

	virtual const SDL_FRect&	getRect() const;
	virtual const SDL_Texture*	getTexture() const;
	virtual int					getHp() const;
	virtual int					getMaxHp() const;
	virtual float				distanceTo(const Entity& a, const Entity& b) const;

	virtual void		setTexture(SDL_Texture* texture);
	virtual void		setPosition(float x, float y);
	virtual void		takeDamage(int damage);
	virtual void		heal(int amount);
	virtual void		setMaxHp(int hp);

protected:
	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;	

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;		// The crop of the original image to render (x,y,w,h)

	int			_hp;
	int			_maxHp;
	float		_speed;
	float		_dirX;
	float		_dirY;
};