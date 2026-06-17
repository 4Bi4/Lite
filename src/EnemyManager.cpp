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
	_spawnRate(1.0f),
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

void	EnemyManager::update(float deltaTimeNS, Game& game)
{
	//	Spawn
	this->_spawnTimer += deltaTimeNS / 1000000000.0f;
	if (this->_spawnTimer >= _spawnRate)
	{
		//	The amount of enemies to spawn
		Uint16 count = (_spawnTimer / _spawnRate);
		for (int i = 0; i < count; i++)
			spawnEnemy(game, DEFAULT);
		this->_spawnTimer = 0.0f;
	}

	//	Check deaths
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (_enemies[i].isActive())
		{
			if (_enemies[i].getHp() <= 0)
			{
				_enemies[i].die();
				_generations[i]++; //	Increment generation to invalidate old EntityIDs
				_enemyCount--;
			}
			_enemies[i].update(deltaTimeNS, game);
		}
	}

	//	Separate overlapping enemies
	resolveCollisions();
}

//	Helper function for colisions
//	Computes the size of the spatial grid cells based on the largest enemy hitbox
float	EnemyManager::computeCellSize() const
{
	//  Each enemy contributes 30% of its hitbox width to the min separation
	//  (0.3 instead of 0.5 to make them overlap a bit)
	const float sepFactor = 0.3f;
	float maxHitW = 0.0f;

	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (!_enemies[i].isActive())
			continue;
		float w = _enemies[i].getHitbox().w;
		if (w > maxHitW)
			maxHitW = w;
	}
	return (maxHitW * sepFactor * 2.0f);
}

//	Helper function for collisions
//	Builds a spatial grid of enemies for efficient collision resolution
void	EnemyManager::buildGrid(std::unordered_map<uint64_t,
			std::vector<size_t> >& grid,
			float invCellSize) const
{
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (!_enemies[i].isActive())
			continue;
		const SDL_FRect& r = _enemies[i].getRect();
		float cx = r.x + r.w * 0.5f;
		float cy = r.y + r.h * 0.5f;
		int32_t gx = (int32_t)std::floor(cx * invCellSize);
		int32_t gy = (int32_t)std::floor(cy * invCellSize);
		grid[packCellKey(gx, gy)].push_back(i);
	}
}

//	Helper function for collisions
//  Push two overlapping enemies apart, splitting the overlap evenly
void	EnemyManager::resolvePair(size_t idxA, size_t idxB, float halfHitA)
{
	//  Fallback push direction on exact overlap (must be a unit vector)
	const float fallbackX = 1.0f;
	const float fallbackY = 0.0f;

	//  Recompute centres live: a prior push may have moved A
	const SDL_FRect& rectA = _enemies[idxA].getRect();
	const SDL_FRect& rectB = _enemies[idxB].getRect();

	float centerAX = rectA.x + rectA.w * 0.5f;
	float centerAY = rectA.y + rectA.h * 0.5f;
	float centerBX = rectB.x + rectB.w * 0.5f;
	float centerBY = rectB.y + rectB.h * 0.5f;

	float dx = centerBX - centerAX;
	float dy = centerBY - centerAY;

	//  Minimum separation = sum of each enemy's own hitbox radius
	float minDist   = halfHitA + _enemies[idxB].getHitbox().w * 0.3f;
	float minDistSq = minDist * minDist;
	float distSq    = dx * dx + dy * dy;

	//  If they dont touch, skip (no sqrt for non-overlaps)
	if (distSq >= minDistSq)
		return;

	float dist = std::sqrt(distSq);
	//  Push direction — fallback to x-axis on exact overlap
	//      (If the distance is 0, we cant divide by it
	//      so we use a default direction of (1, 0))
	float nx = (dist > 0.0f) ? dx / dist : fallbackX;
	float ny = (dist > 0.0f) ? dy / dist : fallbackY;
	//  Split the overlap evenly between both enemies
	float push = (minDist - dist) * 0.5f;

	_enemies[idxA].setPosition(centerAX - nx * push, centerAY - ny * push);
	_enemies[idxB].setPosition(centerBX + nx * push, centerBY + ny * push);
}

//	Helper function for collisions
//  Test one enemy against everyone in its 3x3 block of cells
void    EnemyManager::resolveAgainstNeighbours(
			std::unordered_map<uint64_t, std::vector<size_t> >& grid,
			size_t idxA, int32_t baseX, int32_t baseY)
{
	//  Hitbox width doesnt change when the enemy moves, so hoist it
	float halfHitA = _enemies[idxA].getHitbox().w * 0.3f;

	//  Walk this cell + its 8 neighbours
	for (int32_t oy = -1; oy <= 1; oy++)
	{
		for (int32_t ox = -1; ox <= 1; ox++)
		{
			std::unordered_map<uint64_t, std::vector<size_t> >::iterator
				n = grid.find(packCellKey(baseX + ox, baseY + oy));
			if (n == grid.end())
				continue;

			for (size_t b = 0; b < n->second.size(); b++)
			{
				size_t idxB = n->second[b];
				//  Each pair exactly once (also skips self)
				if (idxB <= idxA)
					continue;
				if (!_enemies[idxB].isActive())
					continue;
				resolvePair(idxA, idxB, halfHitA);
			}
		}
	}
}

//	Helper function for collisions
//  Manages the render position of enemies dont overlap each other
//  Uniform spatial grid: only compares enemies in neighbouring cells ~O(n)
void    EnemyManager::resolveCollisions()
{
	if (_enemies.size() < 2)
		return;

	float cellSize = computeCellSize();
	if (cellSize <= 0.0f)
		return;

	std::unordered_map<uint64_t, std::vector<size_t> > grid;
	grid.reserve(_enemies.size());
	buildGrid(grid, 1.0f / cellSize);

	//  Resolve, testing only the 3x3 block of cells around each one
	for (std::unordered_map<uint64_t, std::vector<size_t> >::const_iterator
			cell = grid.begin(); cell != grid.end(); ++cell)
	{
		int32_t baseX = (int32_t)(cell->first >> 32);
		int32_t baseY = (int32_t)(cell->first & 0xFFFFFFFF);

		for (size_t a = 0; a < cell->second.size(); a++)
		{
			size_t idxA = cell->second[a];
			if (!_enemies[idxA].isActive())
				continue;
			resolveAgainstNeighbours(grid, idxA, baseX, baseY);
		}
	}
}

//	Adds all active enemies to the render queue if they are visible on the screen
void	EnemyManager::addToQueue(Data& data)
{
	Camera* cam = data.getGame()->getCamera();
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		if (_enemies[i].isActive() && cam->isVisible(_enemies[i].getRect()))
			_enemies[i].addToQueue(data);
	}
}

//	Spawn the specified enemy type on a random location on the map
//	(Avoids spawning near the player)
void	EnemyManager::spawnEnemy(Game& game, EnemyType type)
{
	int	index = findAvailableSlot();

	if (index == -1)
	{
		if (Debug::state == true)
			std::cout << RED << "EnemyManager error:" << NO_COLOR << "no room in the enemy pool left" << std::endl;
		return ;
	}

	//	Create new object
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
	texture = TextureManager::loadTexture(path, nullptr);

	//	Get a random spawn position on the map
	float	spawnX, spawnY;

	spawnX = (rand() % (game.getMap()->getWidth() * PIXEL_SIZE));
	spawnY = (rand() % (game.getMap()->getHeight() * PIXEL_SIZE));

	//	Check if the spawn point is too close to the player
	float distanceToPlayer = Entity::distanceTo(*game.getPlayer(), Entity(spawnX, spawnY));
	while (distanceToPlayer < 300.0f)
	{
		//	If it's too close to the player, try again
		spawnX = (rand() % (game.getMap()->getWidth() * PIXEL_SIZE));
		spawnY = (rand() % (game.getMap()->getHeight() * PIXEL_SIZE));
		distanceToPlayer = Entity::distanceTo(*game.getPlayer(), Entity(spawnX, spawnY));
	}

	//	Spawn the new enemy
	_enemies[index].setTexture(texture);
	_enemies[index].setType(type);
	_enemies[index].spawn(newID, spawnX, spawnY);
	_enemyCount++;
}

//	Returns the index of the first available spot
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
	return (-1); // The pool is full
}

//	Removes all enemies from the enemy pool
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
