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
	_target(nullptr),
	_speed(600.0f),
	_dirX(0.0f), _dirY(0.0f) {}

Player::~Player() {}

void	Player::handleGamepadMovement(Data& data)
{
	SDL_Gamepad* g = data.getGamepad();

	if (!g)
		return;

	int rawX = SDL_GetGamepadAxis(g, SDL_GAMEPAD_AXIS_LEFTX);
	int rawY = SDL_GetGamepadAxis(g, SDL_GAMEPAD_AXIS_LEFTY);

	// Normalize the range to -1.0 a 1.0
	if (std::abs(rawX) > DEADZONE)
		_dirX += rawX / MAX_JOYSTICK_VALUE;
	else
		_dirX += 0;

	if (std::abs(rawY) > DEADZONE)
		_dirY += rawY / MAX_JOYSTICK_VALUE;
	else
		_dirY += 0;
}

void	Player::handleKeyboardMovement()
{
	const bool*     keys = SDL_GetKeyboardState(NULL);

	_dirX = 0.0f;
	_dirY = 0.0f;

	if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
			_dirY += -1.0f;
	if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
			_dirY += 1.0f;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
			_dirX += -1.0f;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
			_dirX += 1.0f;
}

void	Player::handleInput(Data& data)
{
	handleKeyboardMovement();
	if (data.getGamepad())
		handleGamepadMovement(data);
}

void	Player::render(Data& data)
{
	//	Get the player's position relative to the camera
	SDL_FRect screenRect = data.camera->apply(_destRect);

	//	Render the player to the screen
	SDL_RenderTextureRotated(data.getRenderer(), _texture, &_srcRect, &screenRect, 0.0, NULL, _flip);
}

void	Player::update(float deltaTime, Data& data)
{
	//	read the input
	handleInput(data);

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
	float maxX = data.map->getWidth() * PIXEL_SIZE;
	float maxY = data.map->getHeight() * PIXEL_SIZE;

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

const SDL_Texture*	Player::getTexture() const
{
	return (_texture);
}

Enemy*	Player::getTarget() const
{
	return (_target);
}

//	Returns a pointer to the closest enemy, or nullptr if there are no enemies
Enemy*	Player::getClosestEnemy(Data& data) const
{
	Enemy* closest = nullptr;
	float closestDist = MAXFLOAT;

	for (auto& enemy : data.enemies)
	{
		float dx = (enemy.getRect().x + enemy.getRect().w / 2.0f) - (_destRect.x + _destRect.w / 2.0f);
		float dy = (enemy.getRect().y + enemy.getRect().h / 2.0f) - (_destRect.y + _destRect.h / 2.0f);
		float dist = std::sqrt(dx * dx + dy * dy);

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = &enemy;
		}
	}

	return (closest);
}

void	Player::setPosition(float x, float y)
{
	_destRect.x = x  - (PIXEL_SIZE / 2.0f);
	_destRect.y = y  - (PIXEL_SIZE / 2.0f);
}

void	Player::setTarget(Enemy* enemy)
{
	_target = enemy;
}

void	Player::setTexture(SDL_Texture* texture)
{
	_texture = texture;
}
