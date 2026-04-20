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

class Player
{
public:
	Player(SDL_Texture* tex);
	Player(SDL_Texture* tex, float x, float y, int w, int h);

	void	HandleInput();
	void	Update(float deltaTime, SDL_Renderer* renderer);
	void	Render(SDL_Renderer* renderer);

private:
	SDL_Texture*	_texture;		// Puntero simple (el Manager se encarga de la memoria)
	SDL_FRect		_destRect;		// Posición y tamaño en pantalla (floats!)
	SDL_FRect		_srcRect;		// El recorte de la imagen original (ints)

	float		_speed = 600.0f;
	float		_dirX = 0.0f;
	float		_dirY = 0.0f;
};