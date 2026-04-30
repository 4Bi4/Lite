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

Weapon::Weapon(std::string name, int damage, int range, Uint64 cooldown) :
_name(name), _damage(damage), _range(range), _cooldown(cooldown) {}

Weapon::~Weapon() {}

//	TODO:
//	For now we assume this is a fireball
//	but this will be the main weapon class and we will
//	different types of attack methods
//	(either making children classes or using an enum to call different methods)
void	Weapon::attack(Entity* target)
{
	static Uint64 lastAttackTime = 0;
	Uint64 currentTime = SDL_GetTicks();

	//	Check if the weapon is off cooldown
	if (currentTime - lastAttackTime >= _cooldown)
	{
		target->takeDamage(_damage);
		if (Debug::state == true)
			std::cout << "Attacking " << target << " with " << _name << " for " << _damage << " damage!\n";
		lastAttackTime = currentTime;
	}
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

Uint64	Weapon::getCooldown() const
{
	return (this->_cooldown);
}
