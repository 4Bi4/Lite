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

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

class Map;

class Map
{
public: 

	Map(SDL_Renderer* renderer, unsigned int height, unsigned int width);
	~Map();

	char**	createMap(unsigned int height, unsigned int width);
	void	DrawMap(SDL_Renderer* renderer);
	void	MapBoundary();

	//	Getters

	char**			getMap() const { return (this->_map); }
	unsigned int	getHeight() const { return (this->_height); }
	unsigned int	getWidth() const { return (this->_width); }

private:

	SDL_Texture*	_grass;
	SDL_Texture*	_wall;

	SDL_FRect		_src;
	SDL_FRect		_dest;

	unsigned int	_height;
	unsigned int	_width;

	char**			_map;
};