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

ProjectileManager::ProjectileManager() : _maxProjectiles(MAX_PROJECTILES)
{
	this->_projectiles.resize(_maxProjectiles, Projectile(BULLET_SMALL, ProjectileStats(), EntityID(), this));
	this->_generations.resize(_maxProjectiles, 0);
}

ProjectileManager::~ProjectileManager()
{
	this->_projectiles.clear();
	this->_generations.clear();
}

void	ProjectileManager::update(float deltaTimeNS, Data& data)
{
	//	Move active projectiles first.
	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		Projectile& projectile = _projectiles[i];
		const bool wasActive = projectile.isActive();
		if (wasActive)
		{
			projectile.update(deltaTimeNS, data);
			if (!projectile.isActive())
				_generations[i]++;
		}
	}

	//	Check collision with the first enemy that intersects
	std::vector<Enemy>& enemies = data.getGame()->getEnemyManager()->getEnemies();
	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		Projectile& projectile = _projectiles[i];
		if (!projectile.isActive())
			continue;

		const SDL_FRect projectileRect = projectile.getHitbox();
		for (size_t j = 0; j < enemies.size(); ++j)
		{
			Enemy& enemy = enemies[j];
			if (!enemy.isActive())
				continue;

			if (SDL_HasRectIntersectionFloat(&projectileRect, &enemy.getHitbox()))
			{
				enemy.takeDamage(projectile.getStats().damage);
				projectile.consumePierce();
				if (projectile.getPierceCount() <= 0)
				{
					projectile.despawn();
					_generations[i]++;
				}
				break;
			}
		}
	}
}

void	ProjectileManager::spawnProjectile(ProjectileType type, ProjectileStats stats, EntityID target, float x, float y)
{
	int index = findAvailableSlot();

	if (index == -1)
	{
		if (Debug::state == true)
			std::cout << RED << "ProjectileManager error:" << NO_COLOR << " no room in the projectile pool left" << std::endl;
		return;
	}

	ProjectileID newID = { index, _generations[index] };
	_projectiles[index].spawn(newID, type, stats, target, x, y);
}

void	ProjectileManager::clearProjectiles()
{
	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		if (_projectiles[i].isActive())
		{
			_projectiles[i].despawn();
			_generations[i]++;
		}
	}
}

void	ProjectileManager::render(Data& data)
{
	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		if (_projectiles[i].isActive())
			_projectiles[i].render(data);
	}
}

int	ProjectileManager::findAvailableSlot()
{
	for (size_t i = 0; i < _projectiles.size(); ++i)
	{
		if (!_projectiles[i].isActive())
			return (i);
	}
	return (-1);
}

std::vector<Projectile>&	ProjectileManager::getProjectiles()
{
	return (_projectiles);
}

Projectile*	ProjectileManager::getProjectile(ProjectileID id, Game& game)
{
	if (!id.isValid() || id.index >= MAX_PROJECTILES)
		return (nullptr);

	ProjectileManager* manager = game.getProjectileManager();
	Projectile& projectile = manager->_projectiles[id.index];

	if (!projectile.isActive() || manager->_generations[id.index] != id.gen)
		return (nullptr);

	return (&projectile);
}
