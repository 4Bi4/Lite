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

Data::Data(void)
	: _player(nullptr), _map(nullptr), _hRes(DEFAULT_HRES), _vRes(DEFAULT_VRES),
	_fpsLimit(DEFAULT_FPS_LIMIT),
	_targetFrameTime(1000 / DEFAULT_FPS_LIMIT),
	_vSync(true), _running(true), _fullscreen(false),
	_sdl() {}

Data::~Data(void)
{
	if (this->_sdl.fontLarge)
		TTF_CloseFont(this->_sdl.fontLarge);
	if (this->_sdl.fontSmall)
		TTF_CloseFont(this->_sdl.fontSmall);
	if (this->_sdl.renderer)
		SDL_DestroyRenderer(this->_sdl.renderer);
	if (this->_sdl.window)
		SDL_DestroyWindow(this->_sdl.window);
	TTF_Quit();
	SDL_Quit();
}

//	Getters

int		Data::getHres() const
{
	return (this->_hRes);
}

int		Data::getVres() const
{
	return (this->_vRes);
}

bool	Data::getVsync() const
{
	return (this->_vSync);
}

bool	Data::isRunning() const
{
	return (this->_running);
}

int		Data::getFpsLimit() const
{
	return (this->_fpsLimit);
}

float	Data::getTargetFrameTime() const
{
	return (this->_targetFrameTime);
}

bool	Data::isFullscreen() const
{
	return (this->_fullscreen);
}

//	Setters

void	Data::setHres(int hRes)
{
	this->_hRes = hRes;
}

void	Data::setVres(int vRes)
{
	this->_vRes = vRes;
}

void	Data::setVsync(bool vSync)
{
	this->_vSync = vSync;
}

void	Data::setRunning(bool running)
{
	this->_running = running;
}

void	Data::setFpsLimit(int fpsLimit)
{
	this->_fpsLimit = fpsLimit;
	if (fpsLimit == 0)
		this->_targetFrameTime = 0;
	else
		this->_targetFrameTime = 1000 / fpsLimit;
}

void	Data::setFullscreen(bool fullscreen)
{
	this->_fullscreen = fullscreen;
}

// Static member initialization
std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;
