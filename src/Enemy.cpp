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

Enemy::Enemy(SDL_Texture* texture, EnemyManager* manager) :
	Entity(texture),
	_texture(texture),
	_flip(SDL_FLIP_NONE),
	_destRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE },
	_hitbox{ 10.0f, 10.0f, 40.0f, 40.0f },
	_manager(manager),
	_type(DEFAULT),
	_id{ -1, 0 },
	_active(false),
	_hp(30), _maxHp(30),
	_damage(10),
	_speed(100),
	_dirX(0), _dirY(0) {}

Enemy::~Enemy() {}

void	Enemy::addToQueue(Data& data)
{
	if (!_texture)
	{
		if (Debug::state == true)
			std::cerr << B_RED << "[ ERROR ] enemy without texture!" << NO_COLOR << std::endl;
		return ;
	}
	//	Get the enemy's position relative to the camera
	SDL_FRect screenRect = data.getGame()->getCamera()->apply(_destRect);
	//	Submit to the render queue — drawn in Y-sorted order by flush()
	data.getGame()->getRenderQueue()->submit(_texture, _srcRect, screenRect, 0.0, _flip, _destRect.y + _destRect.h);
}

//	(Movement "AI") updates dirX and dirY with the next
//	desired position
void	Enemy::calcNextMove(Game& game)
{
	//	for now we assume only 1 player
	//	and this player is allways going to be the target
	SDL_FRect target = game.getPlayer()->getRect();

	_dirX = target.x - _destRect.x;
	_dirY = target.y - _destRect.y;
}

void	Enemy::update(float deltaTime, Game& game)
{
	//	Call the enemy AI to update _dirX and _dirY
	calcNextMove(game);

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

	//move hitbox with the enemy
	_hitbox.x = _destRect.x + 5.0f;
	_hitbox.y = _destRect.y + 5.0f;

	//	Out of bounds check
	float maxX = game.getMap()->getWidth() * PIXEL_SIZE;
    float maxY = game.getMap()->getHeight() * PIXEL_SIZE;

	if (_destRect.x < 0)
		_destRect.x = 0;
	if (_destRect.y < 0)
		_destRect.y = 0;
	if (_destRect.x + _destRect.w > maxX)
		_destRect.x = maxX - _destRect.w;
	if (_destRect.y + _destRect.h > maxY)
		_destRect.y = maxY - _destRect.h;

	//check if the enemy is colliding with the player
	SDL_FRect playerRect = game.getPlayer()->getRect();
	if (SDL_HasRectIntersectionFloat(&_hitbox, &playerRect))
	{
		//	If colliding, deal damage to the player
		if(!game.getPlayer()->isInvulnerable())
		{
			game.getPlayer()->takeDamage(_damage);
			game.getPlayer()->setInvulnerable(); // Set player invulnerable after taking damage with default value
			if (Debug::state == true)
				std::cout << "PlayerHealth is: " << B_RED << game.getPlayer()->getHp() << NO_COLOR << "      " << std::endl;
		}
	}
}

void	Enemy::setUp(EnemyType type, SDL_Texture* texture)
{
	this->_type = type;
	this->_texture = texture;
}

void	Enemy::spawn(EntityID id, float x, float y)
{
	_id = id;
	_hp = _maxHp; //	Reset HP to max
	_active = true;
	this->setPosition(x, y);
}

void	Enemy::die()
{
	_active = false;
	
}

//	GETTERS

int	Enemy::getHp() const
{
	return (_hp);
}

EntityID	Enemy::getID() const
{
	return (_id);
}

//	Returns 4 floats.
//	"x" and "y" are position
//	"h" and "w" are the size on screen
const SDL_FRect&	Enemy::getRect() const
{
	return (_destRect);
}

const SDL_FRect&	Enemy::getHitbox() const
{
	return (_hitbox);
}

EnemyType	Enemy::getType() const
{
	return (_type);
}

bool	Enemy::isActive() const
{
	return (_active);
}

int	Enemy::getMaxHp() const
{
	return (_maxHp);
}

const SDL_Texture*	Enemy::getTexture() const
{
	return (_texture);
}

//	SETTERS

void	Enemy::heal(int amount)
{
	_hp += amount;
	if (_hp > _maxHp)
		_hp = _maxHp;
}

void	Enemy::setMaxHp(int hp)
{
	_maxHp = hp;
	if (_hp > _maxHp)
		_hp = _maxHp;
}

void	Enemy::takeDamage(int damage)
{
	_hp -= damage;
	if (_hp < 0)
		_hp = 0;
}

void	Enemy::setType(EnemyType newType)
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
