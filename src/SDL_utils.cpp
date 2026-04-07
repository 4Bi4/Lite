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

//	Initializes SDL and its subsystems (SDL_ttf, SDL_image, SDL_mixer)
//	RETURN: 0 on success, 1 on error
int	initSDL()
{
	// 1. Init core SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "SDL_Init: " << SDL_GetError() << "\n";
		return (1);
	}
	// 2. SDL_ttf
	if (TTF_Init() != 0)
	{
		std::cerr << "TTF_Init: " << TTF_GetError() << "\n";
		SDL_Quit();
		return (1);
	}
	// 3. SDL_image (PNG + JPG)
	if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		std::cerr << "IMG_Init: " << IMG_GetError() << "\n";
		TTF_Quit();
		SDL_Quit();
		return (1);
	}
	// 4. SDL_mixer (44100 Hz, stereo, 2048-sample chunks)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		std::cerr << "Mix_OpenAudio: " << Mix_GetError() << "\n";
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
		return (1);
	}
	return (0);
}

//	Helper: prints text centred at (cx, cy) via SDL_ttf
void drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy)
{
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { cx - surf->w / 2, cy - surf->h / 2, surf->w, surf->h };
    SDL_FreeSurface(surf);
    if (tex)
    {
        SDL_RenderCopy(ren, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}
