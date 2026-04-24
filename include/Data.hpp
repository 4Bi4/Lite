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

//	Hold all the SDL-related data
struct SdlData
{
	SDL_Window*		window = nullptr;
	SDL_Renderer*	renderer = nullptr;
	TTF_Font*		fontLarge = nullptr;
	TTF_Font*		fontSmall = nullptr;
};

class Player;
class Camera;
class Map;

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
	bool	getVsync() const;
	bool	isRunning() const;
	int		getFpsLimit() const;
	float	getTargetFrameTime() const;

	//	Setters

	void	setHres(int hres);
	void	setVres(int vres);
	void	setVsync(bool vsync);
	void	setRunning(bool running);
	void	setFpsLimit(int fpsLimit);

	//	SDL Getters

	SDL_Window*		getWindow() const { return (this->_sdl.window); }
	SDL_Renderer*	getRenderer() const { return (this->_sdl.renderer); }
	TTF_Font*		getFontLarge() const { return (this->_sdl.fontLarge); }
	TTF_Font*		getFontSmall() const { return (this->_sdl.fontSmall); }

	//	SDL Setters

	void	setWindow(SDL_Window* window) { this->_sdl.window = window; }
	void	setRenderer(SDL_Renderer* renderer) { this->_sdl.renderer = renderer; }
	void	setFontLarge(TTF_Font* fontLarge) { this->_sdl.fontLarge = fontLarge; }
	void	setFontSmall(TTF_Font* fontSmall) { this->_sdl.fontSmall = fontSmall; }

	// Future: This should be a list of game objects, not just one player

	Player*	_player;
	Camera*	_camera;
	Map*	_map;

private:
	int		_hRes;
	int		_vRes;
	int		_h;
	int		_v;
	int		_fpsLimit;
	float	_targetFrameTime;

	bool	_vSync;
	bool	_running;
	SdlData	_sdl;
};
