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

	void	update(float dt, Data& data, int round);
	void	render(Data& data);

	void	clearEnemies();
	void	spawnEnemy(Data& data, enemyType type);

	void	setSpawnTimer(float timer);

	int						findAvailableSlot();
	std::vector<Enemy>&		getEnemies();	
	static Enemy*			getEnemy(EntityID id, Game& game);

private:

	std::vector<Enemy>	_enemies;
	std::vector<Uint32>	_generations;	//	Used to track generations for EntityIDs

	const int		_maxEnemies;
	float 			_spawnTimer;
};
