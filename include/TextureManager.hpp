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

#include "lite.hpp"

#pragma once

class TextureManager
{
public:
	TextureManager() = delete; // Non-instantiable class

	static SDL_Texture* LoadTexture(const std::string& filePath, SDL_Renderer* renderer)
	{
		if (textureCache.find(filePath) != textureCache.end())
			return textureCache[filePath];

		SDL_Texture* tex = IMG_LoadTexture(renderer, filePath.c_str());
		if (!tex)
			SDL_Log("Error loading texture: %s", SDL_GetError());
		else
			textureCache[filePath] = tex;
		return (tex);
	}

	static void Clean()
	{
		for (auto& pair : textureCache)
			SDL_DestroyTexture(pair.second);

		textureCache.clear();
	}

private:
	static std::unordered_map<std::string, SDL_Texture*> textureCache;
};
