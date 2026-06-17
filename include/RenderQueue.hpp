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

#include "lite_common.hpp"

struct RenderEntry
{
	SDL_Texture*	texture;
	SDL_FRect		srcRect;
	SDL_FRect		screenRect;
	double			angle;
	SDL_FlipMode	flip;
	float			sortKey;	//	World-space bottom Y — lower renders first
};

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	void	submit(SDL_Texture* texture, const SDL_FRect& srcRect, const SDL_FRect& screenRect, double angle, SDL_FlipMode flip, float sortKey);
	void	flush(SDL_Renderer* renderer);

private:
	std::vector<RenderEntry>	_entries;
};
