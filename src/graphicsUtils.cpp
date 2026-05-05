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

//	Renders a solid color background to the screen
void	renderBackground(Data& data, SDL_Color color)
{
	SDL_SetRenderDrawColor(data.getRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderClear(data.getRenderer());
}

//	Fills the back of the screen with a rainbow effect
void	makeBGRainbow(Data& data)
{
	static float	hue = 0.0f;
	const float		step = 0.1f;

	float	c = 1.0f;
	float	x = c * (1.0f - std::fabs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
	float	r = 0.0f;
	float	g = 0.0f;
	float	b = 0.0f;

	if (hue < 60.0f)
		r = c, g = x;
	else if (hue < 120.0f)
		r = x, g = c;
	else if (hue < 180.0f)
		g = c, b = x;
	else if (hue < 240.0f)
		g = x, b = c;
	else if (hue < 300.0f)
		r = x, b = c;
	else
		r = c, b = x;

	SDL_Color	color = {
		static_cast<Uint8>(std::round(r * 255.0f)),
		static_cast<Uint8>(std::round(g * 255.0f)),
		static_cast<Uint8>(std::round(b * 255.0f)),
		255
	};

	renderBackground(data, color);
	hue += step;
	if (hue >= 360.0f)
		hue -= 360.0f;
}

void	loadTextures(Data& data)
{
	if (!TextureManager::loadTexture(DIRT_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] loading texture: map wall" << NO_COLOR << std::endl;
	if (!TextureManager::loadTexture(GRASS_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] loading texture: map grass" << NO_COLOR << std::endl;
	if (!TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] LOADING TEXTURE!: default enemy" << NO_COLOR << std::endl;
	if (!TextureManager::loadTexture(DEBUG_ENEMY_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] LOADING TEXTURE!: debug enemy" << NO_COLOR << std::endl;
	if (!TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] LOADING TEXTURE!: default player" << NO_COLOR << std::endl;
	if (!TextureManager::loadTexture(FIREBALL_TEXTURE, data.getRenderer()))
		std::cerr << B_RED << "[ERROR] LOADING TEXTURE!: projectile fireball" << NO_COLOR << std::endl;
}
