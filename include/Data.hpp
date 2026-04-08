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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

//	Hold all the SDL-related data
typedef struct	s_sdl
{
	SDL_Window*		window = nullptr;
	SDL_Renderer*	renderer = nullptr;
	TTF_Font*		fontLarge = nullptr;
	TTF_Font*		fontSmall = nullptr;
	SDL_Texture*	iconTex = nullptr;
	Mix_Chunk*		sound = nullptr;
}				t_sdl;

//	Data is the main data structure of the engine
//	It holds all the necessary data for the engine to run
class Data
{
public:
	Data(void);
	~Data(void);

	//	Getters
	int		getHres() const;
	int		getVres() const;
	int		getFpsLimit() const;
	float	getTargetFrameTime() const;

	//	Setters
	void	setHres(int hres);
	void	setVres(int vres);
	void	setFpsLimit(int fpsLimit);

	//	SDL Getters
	SDL_Window*		getWindow() const { return (this->_sdl.window); }
	SDL_Renderer*	getRenderer() const { return (this->_sdl.renderer); }
	TTF_Font*		getFontLarge() const { return (this->_sdl.fontLarge); }
	TTF_Font*		getFontSmall() const { return (this->_sdl.fontSmall); }
	SDL_Texture*	getIconTex() const { return (this->_sdl.iconTex); }
	Mix_Chunk*		getSound() const { return (this->_sdl.sound); }

	//	SDL Setters
	void	setWindow(SDL_Window* window) { this->_sdl.window = window; }
	void	setRenderer(SDL_Renderer* renderer) { this->_sdl.renderer = renderer; }
	void	setFontLarge(TTF_Font* fontLarge) { this->_sdl.fontLarge = fontLarge; }
	void	setFontSmall(TTF_Font* fontSmall) { this->_sdl.fontSmall = fontSmall; }
	void	setIconTex(SDL_Texture* iconTex) { this->_sdl.iconTex = iconTex; }
	void	setSound(Mix_Chunk* sound) { this->_sdl.sound = sound; }

private:
	int		_hres;
	int		_vres;
	int		_fpsLimit;
	float	_targetFrameTime;
	t_sdl	_sdl;
};
