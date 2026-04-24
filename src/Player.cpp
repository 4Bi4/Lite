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

#include "../include/Player.hpp"

Player::Player(SDL_Texture* tex) : _texture(tex),
	_destRect{ 100.0f, 100.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE } {}

Player::Player(SDL_Texture* tex, float x, float y, int w, int h)
{
	_texture = tex;
	_destRect = { x, y, (float)w, (float)h };

	float texW, texH;
	SDL_GetTextureSize(tex, &texW, &texH);
	_srcRect = { 0, 0, texW, texH };
}

void Player::HandleInput()
{
	const bool*	keys = SDL_GetKeyboardState(NULL);

	_dirX = 0.0f;
	_dirY = 0.0f;

	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
		_dirY -= _speed;
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
		_dirY += _speed;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
		_dirX -= _speed;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
		_dirX += _speed;
}

void	Player::Render(SDL_Renderer* renderer)
{
	SDL_RenderTexture(renderer, _texture, &_srcRect, &_destRect);
}

void Player::Update(float deltaTime, Data& data)
{
	HandleInput();
	float	length = std::sqrt(_dirX * _dirX + _dirY * _dirY);

	//	Normalize the vector to prevent faster diagonal movement
	if (length > 0)
	{
		_dirX /= length;
		_dirY /= length;
	}

	//	Move the player based on direction, speed and delta time (in seconds)
	_destRect.x += _dirX * _speed * deltaTime / 1000000000.0f;
	_destRect.y += _dirY * _speed * deltaTime / 1000000000.0f;

	//	Out of bounds check
	int winW = data.getHres();
	int winH = data.getVres();

	if (_destRect.x < 0)
		_destRect.x = 0;
	if (_destRect.y < 0)
		_destRect.y = 0;
	if (_destRect.x + _destRect.w > winW)
		_destRect.x = winW - _destRect.w;
	if (_destRect.y + _destRect.h > winH)
		_destRect.y = winH - _destRect.h;

	//	Debug player info
	if (Debug::state == true)
		std::cout << "Player position: (" << _destRect.x << ", " << _destRect.y << ")      \r" << std::flush;
}
