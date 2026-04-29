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

#include "Entity.hpp"

class Data;

class Player : public Entity
{
public:
	Player(SDL_Texture* texture);
	~Player();

	void	handleInput(Data& data);
	void	update(float deltaTime, Data& data);
	void	render(Data& data);

	const SDL_FRect& getRect() const;

	void	setPosition(float x, float y);

protected:
	void	handleKeyboardMovement();
	void	handleGamepadMovement(Data& data);

	SDL_Texture*	_texture;		// Just a pointer (the Manager handles the memory)
	SDL_FlipMode	_flip;	

	SDL_FRect		_destRect;		// Position (x,y) and size on screen (w,h)
	SDL_FRect		_srcRect;		// The crop of the original image to render (x,y,w,h)

	float		_speed;
	float		_dirX;
	float		_dirY;
};