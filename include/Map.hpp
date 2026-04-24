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

#define MAP_WIDTH 30
#define MAP_HEIGHT 20

class Map;
class Camera;

class Map
{
public:
	Map(SDL_Renderer* renderer, unsigned int height, unsigned int width);
	~Map();

	void	drawMap(SDL_Renderer* renderer, Camera* camera);
	char**	createMap(unsigned int height, unsigned int width);

	//	Getters

	char**			getMap() const { return (this->_map); }
	unsigned int	getWidth() const { return (this->_width); }
	unsigned int	getHeight() const { return (this->_height); }

private:

	SDL_Texture*	_wall;
	SDL_Texture*	_grass;

	SDL_FRect		_src;
	SDL_FRect		_dest;

	unsigned int	_width;
	unsigned int	_height;

	char**			_map;
};