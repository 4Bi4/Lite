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

#include <algorithm>

RenderQueue::RenderQueue() {}

RenderQueue::~RenderQueue() {}

//	Submit a render entry to the queue
void	RenderQueue::submit(SDL_Texture* texture, const SDL_FRect& srcRect, const SDL_FRect& screenRect, double angle, SDL_FlipMode flip, float sortKey)
{
	_entries.push_back({ texture, srcRect, screenRect, angle, flip, sortKey });
}

//	Flush the queue and render all entries
void	RenderQueue::flush(SDL_Renderer* renderer)
{
	//	Sort by world-space bottom Y so lower entities render on top (painter's algorithm)
	std::sort(
		_entries.begin(),
		_entries.end(),
		[](const RenderEntry& a, const RenderEntry& b)
			{ return (a.sortKey < b.sortKey); }
	);

	//	Draw all entries in order
	for (const RenderEntry& entry : _entries)
		SDL_RenderTextureRotated(renderer, entry.texture, &entry.srcRect, &entry.screenRect, entry.angle, NULL, entry.flip);

	//	Clear for next frame (keeps allocated memory)
	_entries.clear();
}
