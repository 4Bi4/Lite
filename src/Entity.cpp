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

//	Default entity constructor
Entity::Entity(SDL_Texture* texture) :
	_texture(texture), _flip(SDL_FLIP_NONE),
	_destRect{ 0.0f , 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_hp(100), _maxHp(100),
	_speed(600.0f),
	_dirX(0.0f), _dirY(0.0f) {}

//	Dummy Entity
//	Just to calculate with it
//	Not printable/usable
Entity::Entity(float x, float y) :
	_texture(nullptr), _flip(SDL_FLIP_NONE),
	_destRect{ x, y, 0.0f, 0.0f },
	_srcRect{ 0.0f, 0.0f, 0.0f, 0.0f },
	_hp(0), _maxHp(0),
	_speed(0),
	_dirX(0), _dirY(0) {}

Entity::~Entity() {}

//	Default behavior: do nothing
void	Entity::die() {}

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
float	Entity::distanceTo(const Entity& a, const Entity& b)
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
