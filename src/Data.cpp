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

Data::Data(void) : _hres(DEFAULT_HRES), _vres(DEFAULT_VRES), _fpsLimit(DEFAULT_FPS_LIMIT), _targetFrameTime(1000 / DEFAULT_FPS_LIMIT), _sdl() {}

Data::~Data(void)
{
	if (this->_sdl.iconTex)
		SDL_DestroyTexture(this->_sdl.iconTex);
	if (this->_sdl.fontLarge)
		TTF_CloseFont(this->_sdl.fontLarge);
	if (this->_sdl.fontSmall)
		TTF_CloseFont(this->_sdl.fontSmall);
	if (this->_sdl.sound)
		Mix_FreeChunk(this->_sdl.sound);
	if (this->_sdl.renderer)
		SDL_DestroyRenderer(this->_sdl.renderer);
	if (this->_sdl.window)
		SDL_DestroyWindow(this->_sdl.window);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

//	Getters
int		Data::getHres() const
{
	return (this->_hres);
}

int		Data::getVres() const
{
	return (this->_vres);
}

int		Data::getFpsLimit() const
{
	return (this->_fpsLimit);
}

float	Data::getTargetFrameTime() const
{
	return (this->_targetFrameTime);
}

//	Setters
void	Data::setHres(int hres)
{
	this->_hres = hres;
}

void	Data::setVres(int vres)
{
	this->_vres = vres;
}

void	Data::setFpsLimit(int fpsLimit)
{
	this->_fpsLimit = fpsLimit;
	this->_targetFrameTime = 1000 / fpsLimit;
}
