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

Enemy::Enemy(SDL_Texture* texture) :
	Entity(texture),
	_texture(texture), _flip(SDL_FLIP_NONE),
	_destRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_type(DEFAULT), _speed(150), _dirX(0), _dirY(0) {}

Enemy::~Enemy() {}

void	Enemy::render(Data& data)
{
	//	Get the enemy's position relative to the camera
	SDL_FRect screenRect = data.camera->apply(_destRect);

	//	Render the enemy to the screen
	SDL_RenderTextureRotated(data.getRenderer(), _texture, &_srcRect, &screenRect, 0.0, NULL, _flip);
}

//	(Movement "AI") updates dirX and dirY with the next
//	desired position
void	Enemy::calcNextMove(Data& data)
{
	//	for now we assume only 1 player
	//	and this player is allways going to be the target
	SDL_FRect target = data.player->getRect();

	_dirX = target.x - _destRect.x;
	_dirY = target.y - _destRect.y;
}

void	Enemy::update(float deltaTime, Data& data)
{
	//	!!!!!!!!  IMPORTANT !!!!!!!!
	//	TODO:
	//	Call the enemy AI to update _dirX and _dirY
	calcNextMove(data);

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

	//	Move the enemy based on direction, speed and delta time (in seconds)
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
}

//	GETTERS

//	Returns 4 floats.
//	"x" and "y" are position
//	"h" and "w" are the size on screen
const SDL_FRect&	Enemy::getRect() const
{
	return (_destRect);
}

enemyType	Enemy::getType() const
{
	return (_type);
}

//	SETTERS

const SDL_Texture*	Enemy::getTexture() const
{
	return (_texture);
}

void	Enemy::setType(enemyType newType)
{
	_type = newType;
}

void	Enemy::setPosition(float x, float y)
{
	_destRect.x = x  - (PIXEL_SIZE / 2.0f);
	_destRect.y = y  - (PIXEL_SIZE / 2.0f);
}

void	Enemy::setTexture(SDL_Texture* texture)
{
	_texture = texture;
}
