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

#include "../include/lite.hpp"

SDL_Texture*	TextureManager::loadTexture(const std::string& filePath, SDL_Renderer* renderer)
{
	if (textureCache.find(filePath) != textureCache.end())
		return (textureCache[filePath]);

	SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
	if (!tex)
		SDL_Log("Error loading texture: %s", SDL_GetError());
	else
		textureCache[filePath] = tex;
	return (tex);
}

void	TextureManager::Clean()
{
	for (auto& pair : textureCache)
		SDL_DestroyTexture(pair.second);

	textureCache.clear();
}
