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

//	Data is the main data structure of the engine
//	It holds all the necessary data for the engine to run
class Data
{
public:
	Data(void);
	~Data(void);

	void	clearData();

	//	Getters
	int		getHres() const;
	int		getVres() const;
	int		getFpsLimit() const;
	float	getTargetFrameTime() const;
	t_sdl	getSdl() const;

	//	Setters
	void	setHres(int hres);
	void	setVres(int vres);
	void	setFpsLimit(int fpsLimit);

private:
	int		_hres;
	int		_vres;
	int		_fpsLimit;
	float	_targetFrameTime;
	t_sdl	_sdl;
};
