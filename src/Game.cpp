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

Game::Game(Data& data) :
	_enemyManager(),
	_player(TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, nullptr)),
	_camera(data.getHres(), data.getVres()),
	_map(DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH),
	_roundTimer(0.0f),
	_roundDuration(0),
	_currentRound(0),
	_isPaused(false),
	_isGameOver(false) {}

Game::~Game() {}

int	Game::gameLoop(Data& data)
{
	SDL_Event	event;
	Uint64		lastFrame = SDL_GetTicksNS();
	long long	frameCount = 0;
	long long	totalTime = 0;
	int			fps = 0;

	while (!this->isGameOver())
	{
		Uint64	currentFrame = SDL_GetTicksNS();
		//	Time since last frame in nanoseconds
		Uint64	deltaTime = currentFrame - lastFrame;

		lastFrame = currentFrame;
		totalTime += deltaTime;
		frameCount++;

		//	Clear the screen before rendering
		SDL_RenderClear(data.getRenderer());
	
		//	Handle events
		if (handleEvents(data, event) != 0)
			return (1);

		//	Move game logic to a separate function for better organization
		this->update(deltaTime, data);

		//	Render stuff here
		this->render(data);

		//	Round timer
		drawTextCentered(data.getRenderer(),
			data.getFontLarge(),
			std::to_string((int)this->getRemainingTime()),
			{ 0, 0, 0, 0},
			data.getHres() / 2, 40);

		//	FPS + Enemy counter
		if (Debug::state == true)
		{
			// update only every 10 frames
			if (frameCount % 10 == 0)
				fps = (1.0 / ((double)deltaTime / 1000000000.0));

			//	Draw FPS counter
			drawTextLeftAligned(data.getRenderer(),
				data.getFontSmall(),
				"FPS: " + std::to_string(fps),
				{ 255, 255, 255 , 255},
				10, 10);
			//	Draw Enemy counter

			drawTextLeftAligned(data.getRenderer(),
				data.getFontSmall(),
				"Enemies: " + std::to_string(_enemyManager.getEnemyCount()),
				{ 255, 255, 255 , 255},
				10, 30);
		}

		//	Frame limiting (if vsync is disabled)
		Uint64	targetNS = (Uint64)data.getTargetFrameTime() * 1000000;
		Uint64	frameWorkTime = SDL_GetTicksNS() - currentFrame;
	
		if (data.getFpsLimit() > 0 && frameWorkTime < targetNS)
		{
			SDL_DelayNS(targetNS - frameWorkTime);
		}
		SDL_RenderPresent(data.getRenderer());

		// Check for signal and display debug info if caught
		if (this->displaySignalDebugInfo(frameCount, totalTime, deltaTime))
			break; // Exit the game loop
	}

	// DEBUG OUTPUT
	if (Debug::state == true && frameCount > 0)
	{
		std::cout << "\nvsync is: " << (data.getVsync() ? "enabled" : "disabled") << std::endl;
		std::cout << std::fixed << std::setprecision(0);
		std::cout << "\ntotal frames counted: " << frameCount << "\n";
		std::cout << "average frameTime is: " << (double)totalTime / frameCount  << " ns\n";
		std::cout << "target frametime is:  " << (double)data.getTargetFrameTime() * 1000000.0 << " ns" << std::endl;
	}

	return (0);
}

//	Runs the game logic (movement, entities, etc...)
void	Game::update(float deltaTimeNS, Data& data)
{
	if (_isPaused)
		return;

	//	Update the round timer
	//	Convert nanoseconds to seconds
	_roundTimer += deltaTimeNS / 1000000000.0f;

	//	End of round check
	if (_roundTimer >= _roundDuration)
	{
		//	TODO:
		//  Make some kind of mid round pause
		//	for now we just go next
		nextRound();
	}

	//	Update managers
	_enemyManager.update(deltaTimeNS, *data.getGame());
	_projectileManager.update(deltaTimeNS, *data.getGame());

	//	Update
	_player.update(deltaTimeNS, data);

	//	Attack
	_player.attack(*data.getGame());
	//	TODO:
	//	Add enemy attacks here
}

//	Render logic and camera control
void	Game::render(Data& data)
{
	//	Update camera to follow the player
	_camera.update(_player.getRect(), _map.getWidth(), _map.getHeight());

	//	--- Background ---
	makeBGRainbow(data);
	_map.drawMap(data.getRenderer(), &_camera);

	//	--- Foreground ---
	//	 (rendering queue)
	_enemyManager.addToQueue(data);
	_player.addToQueue(data);
	
	_renderQueue.flush(data.getRenderer());

	//	--- Projectiles (always on top) ---
	_projectileManager.render(data);
}

//	Display comprehensive debug info when signal is caught
bool	Game::displaySignalDebugInfo(long long frameCount, long long totalTime, Uint64 deltaTime)
{
	extern volatile sig_atomic_t g_signalReceived;
	extern volatile sig_atomic_t g_signalNumber;
	
	if (!g_signalReceived)
		return (false);

	// Signal information
	std::string sigName = (g_signalNumber == SIGINT) ? "SIGINT (Ctrl+C)" : 
						(g_signalNumber == SIGTERM) ? "SIGTERM" : "UNKNOWN";
	
	std::cout << B_RED << "\n====== SIGNAL CAUGHT: " << sigName << " ======" << NO_COLOR << std::endl;

	// Game state information
	std::cout << B_YELLOW << "Round: " << NO_COLOR << this->getRound() << std::endl;
	std::cout << B_YELLOW << "Enemies: " << NO_COLOR << _enemyManager.getEnemyCount() << std::endl;
	std::cout << B_YELLOW << "Time Remaining: " << NO_COLOR << (int)this->getRemainingTime() << "s" << std::endl;

	// Player information
	SDL_FRect playerRect = _player.getRect();
	std::cout << B_YELLOW << "Player Position: " << NO_COLOR << "(" << (int)playerRect.x << ", " 
		<< (int)playerRect.y << ")" << std::endl;

	// Frame information
	int currentFps = (frameCount > 0) ? (int)(1.0 / ((double)deltaTime / 1000000000.0)) : 0;
	std::cout << B_YELLOW << "FPS: " << NO_COLOR << currentFps << std::endl;
	std::cout << B_YELLOW << "Total Frames: " << NO_COLOR << frameCount << std::endl;
	std::cout << B_YELLOW << "Avg FrameTime: " << NO_COLOR << (int)((double)totalTime / frameCount) << " ns" << std::endl;

	// Game status
	std::cout << B_YELLOW << "Game Status: " << NO_COLOR << (_isPaused ? "PAUSED" : "RUNNING") << std::endl;
	std::cout << B_RED << "Exiting...\n" << NO_COLOR << std::endl;

	return (true); // Signal handled
}

void	Game::nextRound()
{
	_enemyManager.clearEnemies();
	_currentRound++;
	_roundTimer = 0.0f;

	//	Calculate round time
	_roundDuration = ROUND_TIME + _currentRound * 2.0f - 1.0f; //	-1 to make the first round 20 seconds instead of 21

	//	Cap the round duration to a maximum of 90 seconds
	//	(we put 91 so it displays 90 on the screen, since we cast to int)
	if (_roundDuration > 91.0f)
		_roundDuration = 91.0f;

	//	Calculate spawn rate based on the current round
	//	(It's missing a spawnrate multiplier)
	float rate = 3.0f / (_currentRound + 3.0f);
	_enemyManager.setSpawnRate(rate);

	//	Console output
	std::cout << B_YELLOW << "\n¡ROUND " << _currentRound << ", GO!\n" << NO_COLOR << std::endl;
	if (Debug::state == true)
		std::cout << "Spawn rate: " << rate << "s\n" << std::endl;
}

void	Game::togglePause()
{
	_isPaused = !_isPaused;
	if (Debug::state == true)
	{
		if (_isPaused)
			std::cout << "Game paused." << std::endl;
		else
			std::cout << "Game resumed." << std::endl;
	}
}

void	Game::gameOver()
{
	_isGameOver = true;
	std::cout << B_RED << "¡GAME OVER!" << NO_COLOR << std::endl;
}

bool	Game::isPaused() const
{
	return (_isPaused);
}

bool	Game::isGameOver() const
{
	return (_isGameOver);
}

float	Game::getRemainingTime() const
{
	return (_roundDuration - _roundTimer);
}

unsigned int	Game::getRound() const
{
	return (_currentRound);
}

//	Getters

ProjectileManager*	Game::getProjectileManager()
{
	return (&_projectileManager);
}

EnemyManager*	Game::getEnemyManager()
{
	return (&_enemyManager);
}

Player*	Game::getPlayer()
{
	return (&_player);
}

Camera*	Game::getCamera()
{
	return (&_camera);
}

Map*	Game::getMap()
{
	return (&_map);
}

RenderQueue*	Game::getRenderQueue()
{
	return (&_renderQueue);
}

//	Setters

void	Game::setPlayer(const Player& player)
{
	_player = player;
}

void	Game::setCamera(const Camera& camera)
{
	_camera = camera;
}

void	Game::setMap(const Map& map)
{
	_map = map;
}
