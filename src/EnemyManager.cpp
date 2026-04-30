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
#include "../include/EnemyManager.hpp"

EnemyManager::EnemyManager() : _spawnTimer(0.0f) {}

EnemyManager::~EnemyManager()
{
	this->_enemies.clear();
}

void	EnemyManager::update(float deltaTimeNS, Data& data, int round)
{
	// La dificultad escala con la ronda
	float spawnInterval = std::max(0.2f, 1.5f - (round * 0.1f));

	this->_spawnTimer += deltaTimeNS / 1000000000.0f;
	if (this->_spawnTimer >= spawnInterval) {
		spawnEnemy(data);
		this->_spawnTimer = 0.0f;
	}

	//	Update all enemies
	for (auto& Enemy : this->_enemies)
		Enemy.update(deltaTimeNS, data);
}

void	EnemyManager::render(Data& data)
{
	for (auto& Enemy : this->_enemies)
		Enemy.render(data);
}

void	EnemyManager::spawnEnemy(Data& data)
{
	Enemy newEnemy(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()), this);

	//	Get a random spawn position on the map
	float	spawnX, spawnY;

    spawnX = (rand() % (data.game->map.getWidth() * PIXEL_SIZE));
    spawnY = (rand() % (data.game->map.getHeight() * PIXEL_SIZE));

	newEnemy.setPosition(spawnX, spawnY);

	//	Add the new enemy to the list
	this->_enemies.push_back(newEnemy);
}

void	EnemyManager::setSpawnTimer(float timer)
{
	this->_spawnTimer = timer;
}

std::deque<Enemy>&	EnemyManager::getEnemies()
{
	return (this->_enemies);
}

const std::deque<Enemy>&	EnemyManager::getEnemies() const
{
	return (this->_enemies);
}
