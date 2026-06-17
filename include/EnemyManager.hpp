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

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void	update(float dt, Game& game);
	void	addToQueue(Data& data);

	void	clearEnemies();
	void	spawnEnemy(Game& game, EnemyType type);

	void	setSpawnRate(float rate);

	int						findAvailableSlot();
	std::vector<Enemy>&		getEnemies();	
	static Enemy*			getEnemy(EntityID id, Game& game);
	Uint16					getEnemyCount()	const;

private:
	void	resolveCollisions();
	float	computeCellSize() const;
	void	buildGrid(std::unordered_map<uint64_t, std::vector<size_t> >& grid, float invCellSize) const;
	void	resolvePair(size_t idxA, size_t idxB, float halfHitA);
	void	resolveAgainstNeighbours(std::unordered_map<uint64_t, std::vector<size_t> >& grid, size_t idxA, int32_t baseX, int32_t baseY);

	std::vector<Enemy>	_enemies;
	std::vector<Uint32>	_generations;//	Used to track generations for EntityIDs

	const int		_maxEnemies;
	float 			_spawnTimer;
	float			_spawnRate;
	Uint16			_enemyCount;
};
