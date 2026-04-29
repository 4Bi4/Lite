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

#pragma once

#include "lite_common.hpp"

class Weapon
{
public:
	Weapon(std::string name, int damage, int range, Uint64 cooldown);
	~Weapon();

	void	attack(Entity* target);

	void	setName(const std::string& name);
	void	setDamage(int damage);
	void	setRange(int range);
	void	setCooldown(Uint64 cooldown);

	const std::string&	getName() const;
	int					getDamage() const;
	int					getRange() const;
	Uint64				getCooldown() const;

protected:
	std::string	_name;
	int			_damage;
	int			_range;
	Uint64		_cooldown;
};
