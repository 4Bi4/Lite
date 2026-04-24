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

#include "../include/Map.hpp"
#include "../include/lite.hpp"

Map::Map(SDL_Renderer* renderer, unsigned int height, unsigned int width)
{
	_grass = TextureManager::LoadTexture("./resources/textures/map/grass.png", renderer);
	_wall = TextureManager::LoadTexture("./resources/textures/map/wall.png", renderer);
	_src = {0, 0, PIXEL_SIZE, PIXEL_SIZE};
	_dest = {0, 0, PIXEL_SIZE, PIXEL_SIZE};
	_height = height;
	_width = width;

	_map = createMap(height, width);
}

Map::~Map()
{
	// Destroy textures
	if(_grass)
		SDL_DestroyTexture(_grass);
	if(_wall)
		SDL_DestroyTexture(_wall);

	// Free the map memory
	for (unsigned int i = 0; i < _height; i++)
		delete[] (_map[i]);
	delete[] (_map);
}

char**	Map::createMap(unsigned int height, unsigned int width)
{
	char** newMap = new char*[height];
	for (unsigned int i = 0; i < height; i++)
		newMap[i] = new char[width];

	// Fill the map with grass (0) and walls (1)
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
				newMap[i][j] = 1; // Wall
			else
				newMap[i][j] = 0; // Grass
		}
	}

	return (newMap);
}

void	Map::DrawMap(SDL_Renderer* renderer)
{
	int type = 0;

	for (unsigned int i = 0; i < this->getHeight(); i++)
	{
		for (unsigned int j = 0; j < this->getWidth(); j++)
		{
			type = _map[i][j];
			
			SDL_FRect dest = { j * (float)PIXEL_SIZE, i * (float)PIXEL_SIZE, (float)PIXEL_SIZE, (float)PIXEL_SIZE };

			switch (type)
			{
				case (0):
					SDL_RenderTexture(renderer, _grass, &_src, &dest);
					break;
				case (1):
					SDL_RenderTexture(renderer, _wall, &_src, &dest);
					break;
				default:
					SDL_RenderTexture(renderer, _grass, &_src, &dest);
					break;
			}
		}
	}
}