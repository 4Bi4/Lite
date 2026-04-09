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

//	Initializes SDL windows (windowed and fullscreen) and renderers
//	RETURN: 0 on success, 1 on error
int	initSDLWindow(Data& data)
{
	data.setWindow(SDL_CreateWindow(
		"Lite Engine",
		data.getHres(), data.getVres(),
		SDL_WINDOW_RESIZABLE)
	);
	if (!data.getWindow())
	{
		std::cerr << "SDL_CreateWindow: " << SDL_GetError() << "\n";
		return (1);
	}

	data.setRenderer(SDL_CreateRenderer(data.getWindow(), nullptr));
	if (!data.getRenderer())
	{
		std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << "\n";
		return (1);
	}
	if (!SDL_SetRenderVSync(data.getRenderer(), 1))
		std::cerr << "SDL_SetRenderVSync: " << SDL_GetError() << "\n";
	return (0);
}

//	Initializes SDL and its subsystems (SDL_ttf, SDL_image, SDL_mixer)
//	RETURN: 0 on success, 1 on error
int	initSDLCore(Data& data)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		std::cerr << "SDL_Init: " << SDL_GetError() << "\n";
		return (1);
	}

	if (!TTF_Init())
	{
		std::cerr << "TTF_Init: " << SDL_GetError() << "\n";
		return (1);
	}

	//	DEBUG MODE
	if (Debug::state == true)
	{
		std::cout << "init values:\n" << "\n";
		std::cout << "  horizontal resolution: " << data.getHres() << "\n";
		std::cout << "  vertical resolution: " << data.getVres() << "\n";
		std::cout << "  FPS limit: " << data.getFpsLimit() << "\n" << std::endl;
	}
	return (0);
}

//	Initializes SDL (core + window)
//	RETURN: 0 on success, 1 on error
int	initSDL(Data& data)
{
	if (initSDLCore(data) != 0)
		return (1);
	if (initSDLWindow(data) != 0)
		return (1);
	if (Debug::state == true)
		std::cout << B_GREEN << " SDL " << GREEN "initialized successfully!" << NO_COLOR << std::endl;
	return (0);
}

//	Helper: prints text centred at (cx, cy) via SDL_ttf
void drawText(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy)
{
	SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
    if (!surf)
        return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FRect dst = {
		static_cast<float>(cx - surf->w / 2),
		static_cast<float>(cy - surf->h / 2),
		static_cast<float>(surf->w),
		static_cast<float>(surf->h)
	};
    SDL_DestroySurface(surf);
    if (tex)
    {
        SDL_RenderTexture(ren, tex, nullptr, &dst);
        SDL_DestroyTexture(tex);
    }
}
