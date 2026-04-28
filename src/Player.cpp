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

Player::Player(SDL_Texture* texture) :
	Entity(texture),
	_texture(texture), _flip(SDL_FLIP_NONE),
	_destRect{ 0.0f , 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_speed(600.0f),
	_dirX(0.0f), _dirY(0.0f) {}

Player::~Player() {}

void Player::handleInput()
{
	const bool*	keys = SDL_GetKeyboardState(NULL);

	_dirX = 0.0f;
	_dirY = 0.0f;

	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
		_dirY = -1.0f;
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
		_dirY = 1.0f;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
		_dirX = -1.0f;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
		_dirX = 1.0f;
}

void	Player::render(Data& data)
{
	//	Get the player's position relative to the camera
	SDL_FRect screenRect = data._camera->apply(_destRect);

	//	Render the player to the screen
	SDL_RenderTextureRotated(data.getRenderer(), _texture, &_srcRect, &screenRect, 0.0, NULL, _flip);
}

void	Player::update(float deltaTime, Data& data)
{
	//	read the input
	handleInput();

	//	call special physics function HERE (if there is one)

	float	length = std::sqrt(_dirX * _dirX + _dirY * _dirY);

	//	Normalize the vector to prevent faster diagonal movement
	if (length > 0)
	{
		_dirX /= length;
		_dirY /= length;
	}

	//	Check if we need to flip the texture
	if (_dirX < 0)
		_flip = SDL_FLIP_NONE;
	else if (_dirX > 0)
		_flip = SDL_FLIP_HORIZONTAL;

	//	Move the player based on direction, speed and delta time (in seconds)
	_destRect.x += _dirX * _speed * deltaTime / 1000000000.0f;
	_destRect.y += _dirY * _speed * deltaTime / 1000000000.0f;

	//	Out of bounds check
	float maxX = data._map->getWidth() * PIXEL_SIZE;
    float maxY = data._map->getHeight() * PIXEL_SIZE;

	if (_destRect.x < 0)
		_destRect.x = 0;
	if (_destRect.y < 0)
		_destRect.y = 0;
	if (_destRect.x + _destRect.w > maxX)
		_destRect.x = maxX - _destRect.w;
	if (_destRect.y + _destRect.h > maxY)
		_destRect.y = maxY - _destRect.h;

	//	Debug player info
	if (Debug::state == true)
		std::cout << "Player position: (" << _destRect.x << ", " << _destRect.y << ")      \r" << std::flush;
}

//	Returns 4 floats.
//	"x" and "y" are position
//	"h" and "w" are the size on screen
const SDL_FRect&	Player::getRect() const
{
	return (_destRect);
}

void	Player::setPosition(float x, float y)
{
	_destRect.x = x  - (PIXEL_SIZE / 2.0f);
	_destRect.y = y  - (PIXEL_SIZE / 2.0f);
}
