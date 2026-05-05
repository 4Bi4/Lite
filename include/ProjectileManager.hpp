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

class Projectile;

class ProjectileManager
{
public:
	ProjectileManager();
	~ProjectileManager();

	void	update(float deltaTimeNS, Data& data);
	void	render(Data& data);

	void	clearProjectiles();
	void	spawnProjectile(ProjectileType type, ProjectileStats stats, EntityID target, float x, float y);

	int					findAvailableSlot();
	std::vector<Projectile>&	getProjectiles();
	static Projectile*			getProjectile(ProjectileID id, Game& game);

private:
	std::vector<Projectile>	_projectiles;
	std::vector<Uint32>		_generations;//	Used to track generations for ProjectileIDs

	const int				_maxProjectiles;
};
