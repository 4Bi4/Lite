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

Weapon::Weapon(WeaponType type, Entity* holder) :
	_type(type), _lastAttackTime(0), _holder(holder)
{
	switch (type)
	{
		case (FIREBALL) :
		{
			_name = std::string("Fireball!!");
			_range = 300;
			_damage = 10;
			_cooldown = 1000;
			_projectile = FIRE;
			break ;
		}
		default :
		{
			_name = std::string("Unknown");
			_range = 300;
			_damage = 10;
			_cooldown = 1000;
			_projectile = BULLET_SMALL;
		}
	}
}

Weapon::~Weapon() {}

//	TODO:
//	For now we assume this is a fireball
//	but this will be the main weapon class and we will
//	different types of attack methods
//	(either making children classes or using an enum to call different methods)
void	Weapon::attack(Enemy* target, Game& game)
{
	Uint64 currentTime = SDL_GetTicks();

	//	Check if the weapon is off cooldown
	if (currentTime - _lastAttackTime >= (Uint64)_cooldown)
	{
		ProjectileStats stats = getProjectileStats(_holder, _projectile);
		switch (_type)
		{
			case (FIREBALL) :
				shootFireball(game, target, stats);
				break ;
			default :
				std::cerr << RED << "No weapon type! impossible to attack " << NO_COLOR << _name << std::endl;
		}
		if (Debug::state == true)
			std::cout << "Attacking " << target << " with " << _name << " for " << _damage << " damage!\n";
		_lastAttackTime = currentTime;
	}
}

ProjectileStats	Weapon::getProjectileStats(Entity* holder, ProjectileType type)
{
	(void)holder;
	(void)type;
	//	TODO:
	//	Get the holders properties
	//	(buffs and debuffs)

	//	TESTING:
	//	For now we hardcode the values
	
	ProjectileStats stats = { _damage, 400, 1, 0, 1.0f };
	return (stats);
}

void	Weapon::shootFireball(Game& game, Enemy* target, ProjectileStats stats) const
{
	if (!_holder || !target)
		return;

	const SDL_FRect& holderRect = _holder->getRect();
	const float spawnX = holderRect.x + (holderRect.w * 0.5f);
	const float spawnY = holderRect.y + (holderRect.h * 0.5f);

	game.getProjectileManager()->spawnProjectile(_projectile, stats, target->getID(), spawnX, spawnY);
}

void	Weapon::setName(const std::string& name)
{
	this->_name = name;
}

void	Weapon::setDamage(int damage)
{
	this->_damage = damage;
}

void	Weapon::setRange(int range)
{
	this->_range = range;
}

void	Weapon::setCooldown(Uint64 cooldown)
{
	this->_cooldown = cooldown;
}

const std::string&	Weapon::getName() const
{
	return (this->_name);
}

int	Weapon::getDamage() const
{
	return (this->_damage);
}

int	Weapon::getRange() const
{
	return (this->_range);
}

int	Weapon::getCooldown() const
{
	return (this->_cooldown);
}
