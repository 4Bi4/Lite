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

#include "lite.hpp"

class Data;

class Player
{
public:
	Player(SDL_Texture* texture);
	~Player();

	void	handleInput();
	void	update(float deltaTime, Data& data);
	void	render(Data& data);

	const SDL_FRect& getRect() const;

protected:
	SDL_Texture*	_texture;		// Puntero simple (el Manager se encarga de la memoria)
	SDL_FRect		_destRect;		// Posición y tamaño en pantalla
	SDL_FRect		_srcRect;		// El recorte de la imagen original

	float		_speed;
	float		_dirX;
	float		_dirY;
	float		_posX;
	float		_posY;
};