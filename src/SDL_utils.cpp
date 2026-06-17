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

//	Initializes SDL window and renderer (windowed and fullscreen)
//	\returns
//	0 on success, 1 on error
int	initSDLWindow(Data& data)
{
	//	Create window
	data.setWindow(SDL_CreateWindow("Lite Engine", data.getHres(), data.getVres(), SDL_WINDOW_RESIZABLE));
	if (!data.getWindow())
	{
		std::cerr << "SDL_CreateWindow: " << SDL_GetError() << "\n";
		return (1);
	}
	//	Set fullscreen if needed
	if (data.isFullscreen())
	{
		if (SDL_SetWindowFullscreen(data.getWindow(), SDL_WINDOW_FULLSCREEN) != true)
		{
			std::cerr << "SDL_SetWindowFullscreen: " << SDL_GetError() << "\n";
			data.setFullscreen(false); // Revert state on failure
		}
	}

	//	Create renderer
	data.setRenderer(SDL_CreateRenderer(data.getWindow(), nullptr));
	if (!data.getRenderer())
	{
		std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << "\n";
		return (1);
	}
	//	Set vsync
	if (data.getVsync() == false)
	{
		SDL_SetRenderVSync(data.getRenderer(), SDL_RENDERER_VSYNC_DISABLED);
		SDL_SetWindowSurfaceVSync(data.getWindow(), SDL_WINDOW_SURFACE_VSYNC_DISABLED);
	}
	else
	{
		SDL_SetRenderVSync(data.getRenderer(), 1);
		SDL_SetWindowSurfaceVSync(data.getWindow(), 1);
	}
	return (0);
}

//	Initializes SDL and its subsystems (SDL_ttf, SDL_image, SDL_mixer)
//	\returns
//	0 on success, 1 on error
int	initSDLCore(Data& data)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
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

//	Initializes SDL_ttf and loads fonts
//	\returns
//	0 on success, 1 on error
int	initSDLText(Data& data)
{
	//	Load fonts
	data.setFontLarge(TTF_OpenFont("resources/fonts/comic_sans/comicbd.ttf", 62));
	if (!data.getFontLarge())
	{
		std::cerr << "TTF_OpenFont: " << SDL_GetError() << "\n";
		return (1);
	}

	data.setFontSmall(TTF_OpenFont("resources/fonts/TlwgTypo/TlwgTypo-Bold.ttf", 16));
	if (!data.getFontSmall())
	{
		std::cerr << "TTF_OpenFont: " << SDL_GetError() << "\n";
		return (1);
	}
	return (0);
}

//	Initializes SDL (core + window)
//	\returns
//	0 on success, 1 on error
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

//	Helper: prints text left-aligned at (x, y) via SDL_ttf
void	drawTextLeftAligned(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int x, int y)
{
	SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
	if (!surf)
		return;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FRect dst = {
		static_cast<float>(x),
		static_cast<float>(y),
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

//	Helper: prints text centred at (cx, cy) via SDL_ttf
void	drawTextCentered(SDL_Renderer* ren, TTF_Font* font, const std::string& text, SDL_Color color, int cx, int cy)
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
