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
	_target{ -1, 0 },
	_weapon(nullptr),
	_hp(100), _maxHp(100),
	_isInvulnerable(false),
	_lastDamageTime(0),
	_speed(400.0f),
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
	float maxX = data.getGame()->getMap()->getWidth() * PIXEL_SIZE;
	float maxY = data.getGame()->getMap()->getHeight() * PIXEL_SIZE;

	if (_destRect.x < 0)
		_destRect.x = 0;
	if (_destRect.y < 0)
		_destRect.y = 0;
	if (_destRect.x + _destRect.w > maxX)
		_destRect.x = maxX - _destRect.w;
	if (_destRect.y + _destRect.h > maxY)
		_destRect.y = maxY - _destRect.h;

	updateInvulnerabilityTimer();

	//	Update the player's target
	EntityID newTarget = getClosestEnemy(*data.getGame()->getEnemyManager());
	setTarget(newTarget, *data.getGame());

	//	Debug player info
	if (Debug::state == true)
		std::cout << "Player position: (" << _destRect.x << ", " << _destRect.y << ")      \r" << std::flush;
}

void	Player::addToQueue(Data& data)
{
	//	Get the player's position relative to the camera
	SDL_FRect screenRect = data.getGame()->getCamera()->apply(_destRect);

	//	Submit to the render queue — drawn in Y-sorted order by flush()
	data.getGame()->getRenderQueue()->submit(_texture, _srcRect, screenRect, 0.0, _flip, _destRect.y + _destRect.h);
}

//	Attacks the player's target with their weapon
void	Player::attack(Game& game) const
{
	if (!_weapon || !_target.isValid())
		return;

	Enemy* target = EnemyManager::getEnemy(_target, game);
	if (!target)
		return;

	float	distToTarget = Entity::distanceTo(*this, *target);

	//	TODO:
	//	implement multiple weapons if desired
	//  (for now the player can only have 1 weapon)
	if (distToTarget <= _weapon->getRange())
		_weapon->attack(target, game);
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

EntityID	Player::getTarget() const
{
	return (_target);
}

Weapon*	Player::getWeapon() const
{
	return (_weapon);
}

//	Returns the index of the closest enemy, or nullptr if there are no enemies
EntityID	Player::getClosestEnemy(EnemyManager& enemyManager) const
{
	const std::vector<Enemy>& enemies = enemyManager.getEnemies();	

	EntityID closest = { -1, 0 };
	float	closestDist = MAXFLOAT;
	
	for (size_t i = 0; i < enemyManager.getEnemies().size(); i++)
	{
		//	Skip the non used enemies
		if (!enemies[i].isActive())
			continue;

		float dist = Entity::distanceTo(*this, enemies[i]);

		if (dist < closestDist)
		{
			closestDist = dist;
			closest = enemies[i].getID();
		}
	}
	return (closest);
}

int	Player::getHp() const
{
	return (_hp);
}

int	Player::getMaxHp() const
{
	return (_maxHp);
}

bool Player::isInvulnerable() const
{
	return _isInvulnerable;
}

void	Player::setPosition(float x, float y)
{
	_destRect.x = x  - (PIXEL_SIZE / 2.0f);
	_destRect.y = y  - (PIXEL_SIZE / 2.0f);
}

void	Player::setTarget(EntityID enemy, Game& game)
{
	if (!enemy.isValid() || enemy == _target)
		return;
	
	//	Shiny enemy outline for debug mode
	if (Debug::state == true)
	{
		//enemy->setTexture(TextureManager::loadTexture(DEBUG_ENEMY_TEXTURE, nullptr));
		Enemy *oldTarget = EnemyManager::getEnemy(_target, game);
		if (oldTarget)
			oldTarget->setTexture(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, nullptr));
		Enemy* newTarget = EnemyManager::getEnemy(enemy, game);
		if (newTarget)
			newTarget->setTexture(TextureManager::loadTexture(DEBUG_ENEMY_TEXTURE, nullptr));
	}
	_target = enemy;
}

void	Player::setTexture(SDL_Texture* texture)
{
	_texture = texture;
}

void	Player::setWeapon(Weapon* weapon)
{
	_weapon = weapon;
}

void	Player::takeDamage(int damage)
{
	_hp -= damage;
	if (_hp < 0)
		_hp = 0;
}

void	Player::heal(int amount)
{
	_hp += amount;
	if (_hp > _maxHp)
		_hp = _maxHp;
}

void	Player::setMaxHp(int hp)
{
	_maxHp = hp;
	if (_hp > _maxHp)
		_hp = _maxHp;
}

void Player::setInvulnerable(int invulnerable)
{
	_invulnerabilityTimer = invulnerable;
	_isInvulnerable = true;
	_lastDamageTime = SDL_GetTicks();
}

void Player::setInvulnerable()
{
	_invulnerabilityTimer = 500; // Default invulnerability time
	_isInvulnerable = true;
	_lastDamageTime = SDL_GetTicks();
}

void Player::updateInvulnerabilityTimer()
{
	if (_isInvulnerable)
	{
		Uint64 currentTime = SDL_GetTicks();
		if (currentTime - _lastDamageTime >= _invulnerabilityTimer)
			_isInvulnerable = false;
	}
}
