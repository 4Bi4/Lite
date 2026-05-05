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

//	Returns a pointer to the loaded texture
//	If the texture was already loaded, it returns the cached texture
//	\returns
//	A pointer to the loaded texture,
//	or nullptr if the texture failed to load 
SDL_Texture*	TextureManager::loadTexture(const std::string& filePath, SDL_Renderer* renderer)
{
	//	Check if the texture is already loaded
	//	If it is, return the cached texture
	if (textureCache.find(filePath) != textureCache.end())
		return (textureCache[filePath]);

	if (!renderer)
		return (nullptr);

	SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
	if (!tex)
		SDL_Log("Error loading texture: %s", SDL_GetError());
	else
		textureCache[filePath] = tex;

	//	Set the scale mode for pixel art (SDL_SCALEMODE_NEAREST)
	//	for non pixel art it would be (SDL_SCALEMODE_LINEAR)
	SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
	return (tex);
}

//	Cleans up all loaded textures from memory
void	TextureManager::Clean()
{
	for (auto& pair : textureCache)
		SDL_DestroyTexture(pair.second);

	textureCache.clear();
}
