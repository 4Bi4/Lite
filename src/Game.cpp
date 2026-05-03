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
	_player(TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, data.getRenderer())),
	_camera(data.getHres(), data.getVres()),
	_map(data.getRenderer(), DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH),
	_roundTimer(0.0f),
	_roundDuration(ROUND_TIME),
	_currentRound(1),
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
		drawText(data.getRenderer(),
			data.getFontLarge(),
			std::to_string((int)this->getRemainingTime()),
			{ 0, 0, 0, 0},
			data.getHres() / 2, 40);

		//	FPS counter
		if (Debug::state == true)
		{
			// update only every 10 frames
			if (frameCount % 10 == 0)
				fps = (1.0 / ((double)deltaTime / 1000000000.0));

			drawText(data.getRenderer(),
				data.getFontSmall(),
				"FPS: " + std::to_string(fps),
				{ 255, 255, 255 , 255},
				45, 10);
		}

		//	Frame limiting (if vsync is disabled)
		Uint64	targetNS = (Uint64)data.getTargetFrameTime() * 1000000;
		Uint64	frameWorkTime = SDL_GetTicksNS() - currentFrame;
	
		if (data.getFpsLimit() > 0 && frameWorkTime < targetNS)
		{
			SDL_DelayNS(targetNS - frameWorkTime);
		}
		SDL_RenderPresent(data.getRenderer());
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

	return(0);
}

//	Runs the game logic (movement, entities, etc...)
void	Game::update(float deltaTimeNS, Data& data)
{
	if (_isPaused)
		return;

	//	Update the round timer
	//	Convert nanoseconds to seconds
	_roundTimer += deltaTimeNS / 1000000000.0f;

	//	Update enemies
	_enemyManager.update(deltaTimeNS, data, _currentRound);
	//	Update player
	_player.update(deltaTimeNS, data);
	//	Handle player attacks
	_player.attack(*data.getGame());

	//	End of round check
	if (_roundTimer >= _roundDuration)
	{
		//	TODO:
		//  Make some kind of mid round pause
		//	for now we just go next
		nextRound();
	}
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
	_enemyManager.render(data);
	_player.render(data);
}

void	Game::nextRound()
{
	_enemyManager.clearEnemies();
	_currentRound++;
	_roundTimer = 0.0f;

	//	Increase the wave duration
	_roundDuration += 2.0f;
	_enemyManager.setSpawnTimer(std::abs((ROUND_TIME - _currentRound * 5.0f) / 6.0f));
	std::cout << B_YELLOW << "¡RONDA " << _currentRound << " INICIADA!\n" << NO_COLOR << std::endl;
}

void	Game::togglePause()
{
	_isPaused = !_isPaused;
	if (Debug::state == true)
	{
		if (_isPaused)
			std::cout << "Juego pausado." << std::endl;
		else
			std::cout << "Juego reanudado." << std::endl;
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
