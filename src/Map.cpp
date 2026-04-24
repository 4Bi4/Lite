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
	_wall = TextureManager::LoadTexture("./resources/textures/map/wall.png", renderer);
	_grass = TextureManager::LoadTexture("./resources/textures/map/grass.png", renderer);
	_src = {0, 0, PIXEL_SIZE, PIXEL_SIZE};
	_dest = {0, 0, PIXEL_SIZE, PIXEL_SIZE};
	_height = height;
	_width = width;

	_map = createMap(height, width);
}

Map::~Map()
{
	//	No need to destroy textures here
	//	TextureManager handles that :p

	//	Free the map memory
	for (unsigned int i = 0; i < _height; i++)
		delete[] (_map[i]);
	delete[] (_map);
}

char**	Map::createMap(unsigned int height, unsigned int width)
{
	char** newMap = new char*[height];
	for (unsigned int i = 0; i < height; i++)
		newMap[i] = new char[width];

	//	Fill the map with grass (0) and walls (1)
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

void	Map::drawMap(SDL_Renderer* renderer, Camera* camera)
{
	if (!camera)
		return;

	const SDL_FRect& view = camera->getView();

	//	Calculate visible tile range based on camera position and size
	//	(Add -1 and +1 to include partially visible tiles at the edges)
	int startCol = std::max(0, (int)(view.x / PIXEL_SIZE));
	int startRow = std::max(0, (int)(view.y / PIXEL_SIZE));
	
	int endCol   = std::min((int)this->getWidth(), (int)((view.x + view.w) / PIXEL_SIZE) + 1);
	int endRow   = std::min((int)this->getHeight(), (int)((view.y + view.h) / PIXEL_SIZE) + 1);

	for (int i = startRow; i < endRow; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			int type = _map[i][j];
			
			//	1. Calculate the world position of the tile
			SDL_FRect worldDest = { 
				j * (float)PIXEL_SIZE, 
				i * (float)PIXEL_SIZE, 
				(float)PIXEL_SIZE, 
				(float)PIXEL_SIZE 
			};

			//	2. Transform to screen position using the camera
			SDL_FRect screenDest = camera->apply(worldDest);

			//	3. Render based on the tile type
			switch (type)
			{
				case (0): // Grass
				{
					SDL_RenderTexture(renderer, _grass, &_src, &screenDest);
					break;
				}
				case (1): // Wall
				{
					SDL_RenderTexture(renderer, _wall, &_src, &screenDest);
					break;
				}
				default:
					break;
			}
		}
	}
}