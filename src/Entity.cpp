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

Entity::Entity(SDL_Texture* texture) :
	_texture(texture), _flip(SDL_FLIP_NONE),
	_destRect{ 0.0f , 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_hp(100), _maxHp(100),
	_speed(600.0f),
	_dirX(0.0f), _dirY(0.0f) {}

Entity::~Entity() {}

void	Entity::render(Data& data)
{
	//	Get the Entity's position relative to the camera
	SDL_FRect screenRect = data.game->camera.apply(_destRect);

	//	Render the Entity to the screen
	SDL_RenderTextureRotated(data.getRenderer(), _texture, &_srcRect, &screenRect, 0.0, NULL, _flip);
}

void	Entity::update(float deltaTime, Data& data)
{
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

	//	Move the Entity based on direction, speed and delta time (in seconds)
	_destRect.x += _dirX * _speed * deltaTime / 1000000000.0f;
	_destRect.y += _dirY * _speed * deltaTime / 1000000000.0f;

	//	Out of bounds check
	float maxX = data.game->map.getWidth() * PIXEL_SIZE;
    float maxY = data.game->map.getHeight() * PIXEL_SIZE;

	if (_destRect.x < 0)
		_destRect.x = 0;
	if (_destRect.y < 0)
		_destRect.y = 0;
	if (_destRect.x + _destRect.w > maxX)
		_destRect.x = maxX - _destRect.w;
	if (_destRect.y + _destRect.h > maxY)
		_destRect.y = maxY - _destRect.h;
}

//	Returns 4 floats.
//	"x" and "y" are position
//	"h" and "w" are the size on screen
const SDL_FRect&	Entity::getRect() const
{
	return (_destRect);
}

const SDL_Texture*	Entity::getTexture() const
{
	return (_texture);
}

int	Entity::getHp() const
{
	return (_hp);
}

int	Entity::getMaxHp() const
{
	return (_maxHp);
}

void	Entity::setPosition(float x, float y)
{
	_destRect.x = x  - (PIXEL_SIZE / 2.0f);
	_destRect.y = y  - (PIXEL_SIZE / 2.0f);
}

void	Entity::setTexture(SDL_Texture* texture)
{
	_texture = texture;
}

//	Returns the distance between two entities
float	Entity::distanceTo(const Entity& a, const Entity& b) const
{
	const float distanceX = a.getRect().x - b.getRect().x;
	const float distanceY = a.getRect().y - b.getRect().y;

	const float distance = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

	return (distance);
}

void	Entity::takeDamage(int damage)
{
	_hp -= damage;
	if (_hp < 0)
		_hp = 0;
}

void	Entity::heal(int amount)
{
	_hp += amount;
	if (_hp > _maxHp)
		_hp = _maxHp;
}

void	Entity::setMaxHp(int hp)
{
	_maxHp = hp;
	if (_hp > _maxHp)
		_hp = _maxHp;
}
