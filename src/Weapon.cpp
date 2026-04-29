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

void	Weapon::attack(Entity* target)
{
	static Uint64 lastAttackTime = 0;
	if (SDL_GetTicks() - lastAttackTime >= _cooldown)
	{
		//	Here you would implement the actual attack logic, like checking range, applying damage, etc.
		if (Debug::state == true)
			std::cout << "Attacking " << target << " with " << _name << " for " << _damage << " damage!\n";
		lastAttackTime = SDL_GetTicks();
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
