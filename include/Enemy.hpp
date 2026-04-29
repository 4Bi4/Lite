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

class Enemy : public Entity
{
public:
	Enemy(SDL_Texture* texture);
	~Enemy();

	void	calcNextMove(Data& data);
	void	update(float deltaTime, Data& data);
	void	render(Data& data);

	const SDL_FRect&	getRect() const;
	enemyType			getType() const;
	const SDL_Texture*	getTexture() const;


	void	setType(enemyType type);
	void	setTexture(SDL_Texture* texture);
	void	setPosition(float x, float y);


protected:
	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;		// The crop of the original image to render (x,y,w,h)

	enemyType		_type;

	float			_speed;
	float			_dirX;
	float			_dirY;
};
