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

EnemyManager::EnemyManager() : _maxEnemies(MAX_ENEMIES),
	_spawnTimer(0.0f),
	_spawnRate(2.0f),
	_enemyCount(0)
{
	_enemies.resize(_maxEnemies, Enemy(nullptr, this));
	_generations.resize(_maxEnemies, 0);
}

EnemyManager::~EnemyManager()
{
	this->_enemies.clear();
	this->_generations.clear();
}

void	EnemyManager::update(float deltaTimeNS, Data& data)
{
	//	Spawn
	this->_spawnTimer += deltaTimeNS / 1000000000.0f;
	if (this->_spawnTimer >= _spawnRate)
	{
		//	The amount of enemies to spawn
		Uint16 count = (_spawnTimer / _spawnRate);
		for (int i = 0; i < count; i++)
			spawnEnemy(data, DEFAULT);
		this->_spawnTimer = 0.0f;
	}

	//	TODO:
	//	Update stats here
	//	(kills or whatever we want)
	//	  |  |  |  |  |  |  |  |
	//	  V  V  V  V  V  V  V  V

	//	Check deaths
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (_enemies[i].isActive() && _enemies[i].getHp() <= 0)
		{
			_enemies[i].die();
			_generations[i]++; //	Increment generation to invalidate old EntityIDs
			_enemyCount--;
		}
	}

	//	Update all enemies
	for (size_t i = 0; i < _enemies.size(); i++)
		_enemies[i].update(deltaTimeNS, data);
}

void	EnemyManager::render(Data& data)
{
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (_enemies[i].isActive())
			_enemies[i].render(data);
	}
}

void	EnemyManager::spawnEnemy(Data& data, enemyType type)
{
	int	index = findAvailableSlot();

	if (index == -1)
	{
		if (Debug::state == true)
			std::cout << RED << "EnemyManager error:" << NO_COLOR << "no room in the enemy pool left" << std::endl;
		return ;
	}

	//	Create object
	EntityID newID = { index, _generations[index]};

	//	Get the texture
	SDL_Texture* texture;
	std::string  path;

	switch (type)
	{
		case DEFAULT
			: path = DEFAULT_ENEMY_TEXTURE;
				break;
		//	Other enemy types here
		default
			: path = DEFAULT_PLAYER_TEXTURE;
	}

	//	Load the texture (or get it from the cache if it's already loaded)
	texture = TextureManager::loadTexture(path, data.getRenderer());

	//	Get a random spawn position on the map
	float	spawnX, spawnY;

	spawnX = (rand() % (data.getGame()->getMap()->getWidth() * PIXEL_SIZE));
	spawnY = (rand() % (data.getGame()->getMap()->getHeight() * PIXEL_SIZE));

	while (Entity::distanceTo(*data.getGame()->getPlayer(), Enemy(texture, this)) < 600.0f)
	{
		//	If the spawn point is too close to the player, try again
		spawnX = (rand() % (data.getGame()->getMap()->getWidth() * PIXEL_SIZE));
		spawnY = (rand() % (data.getGame()->getMap()->getHeight() * PIXEL_SIZE));
	}

	//	Spawn the new enemy
	_enemies[index].setTexture(texture);
	_enemies[index].setType(type);
	_enemies[index].spawn(newID, spawnX, spawnY);
	_enemyCount++;
}

//	Retuns the index of the first available spot
//	on the enemy pool
//	\returns
//	and int from 0 to MAX_ENEMIES,
//	-1 if the pool is full
int	EnemyManager::findAvailableSlot()
{
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (!_enemies[i].isActive())
			return (i);
	}
	return (-1); // Total chaos: the pool is full!
}

void	EnemyManager::clearEnemies()
{
	for (size_t i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies[i].isActive())
		{
			this->_enemies[i].die();
			this->_generations[i]++;
		}
	}
	_enemyCount = 0;
}

void	EnemyManager::setSpawnRate(float rate)
{
	this->_spawnRate = rate;
}

std::vector<Enemy>&	EnemyManager::getEnemies()
{
	return (this->_enemies);
}

Enemy*	EnemyManager::getEnemy(EntityID id, Game& game)
{
	if (!id.isValid() || id.index >= MAX_ENEMIES)
		return (nullptr);

	EnemyManager* manager = game.getEnemyManager();
	Enemy& enemy = manager->_enemies[id.index];

	if (!enemy.isActive() || manager->_generations[id.index] != id.gen)
		return (nullptr);

	return (&enemy);
}

Uint16	EnemyManager::getEnemyCount() const
{
	return (_enemyCount);
}
