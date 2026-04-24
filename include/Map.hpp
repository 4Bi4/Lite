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

#define MAP_WIDTH 15
#define MAP_HEIGHT 15

class Map;
class Camera;

class Map
{
public: 

	Map(SDL_Renderer* renderer, unsigned int height, unsigned int width);
	~Map();

	char**	createMap(unsigned int height, unsigned int width);
	void	drawMap(SDL_Renderer* renderer, Camera* camera);

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