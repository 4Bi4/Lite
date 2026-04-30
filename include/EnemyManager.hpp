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

#include <deque>

#include "lite_common.hpp"

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void	update(float dt, Data& data, int round);
	void	render(Data& data);

	void	spawnEnemy(Data& data);

	void	setSpawnTimer(float timer);

	std::deque<Enemy>&			getEnemies();
	const std::deque<Enemy>&	getEnemies() const;

private:
	//	Using a deque because we will be adding and removing enemies frequently
	//	and deques dont move elements in memory
	//	so we can safely store pointers to enemies
	std::deque<Enemy>	_enemies;

	float _spawnTimer;
};
