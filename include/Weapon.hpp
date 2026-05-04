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

#include "Projectile.hpp"

class Game;
class Enemy;

class Weapon
{
public:
	Weapon(WeaponType type, Entity* holder);
	~Weapon();

	void	attack(Enemy* target, Game& game);

	void	setName(const std::string& name);
	void	setRange(int range);
	void	setDamage(int damage);
	void	setCooldown(Uint64 cooldown);

	const std::string&	getName() const;
	int					getRange() const;
	int					getDamage() const;
	int					getCooldown() const;

private:
	ProjectileStats	getProjectileStats(Entity* holder, ProjectileType type);

	void	shootFireball(Game& game, Enemy* target, ProjectileStats stats) const;

	std::string		_name;
	WeaponType		_type;
	int				_range;
	int				_damage;
	int				_cooldown;//	Cooldown in milliseconds
	ProjectileType	_projectile;
	Uint64			_lastAttackTime;

	Entity*			_holder;
};
