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
	enemyManager(),
	player(TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, data.getRenderer())),
	camera(data.getHres(), data.getVres()),
	map(data.getRenderer(), DEFAULT_MAP_HEIGHT, DEFAULT_MAP_WIDTH),
	_roundTimer(0.0f),
	_roundDuration(ROUND_TIME),
	_currentRound(0),
	_isPaused(false) {}

Game::~Game() {}

//	Runs the game logic (movement, entities, etc...)
void	Game::update(float deltaTimeNS, Data& data)
{
	if (_isPaused)
		return;

	//	Update the round timer
	//	Convert nanoseconds to seconds
	_roundTimer += deltaTimeNS / 1000000000.0f;

	//	Update enemies
	enemyManager.update(deltaTimeNS, data, _currentRound);
	//	Update player
	data.game->player.update(deltaTimeNS, data);
	//	Handle player attacks
	data.game->player.attack();

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
	camera.update(player.getRect(), map.getWidth(), map.getHeight());

	//	--- Background ---
	makeBGRainbow(data);
	map.drawMap(data.getRenderer(), &camera);

	//	--- Foreground ---
	enemyManager.render(data);
	player.render(data);
}

void	Game::nextRound()
{
	_currentRound++;
	_roundTimer = 0.0f;

	//	Increase the wave duration
	_roundDuration += 5.0f;
	enemyManager.setSpawnTimer((ROUND_TIME - _currentRound * 5.0f) / 6.0f);
	std::cout << "¡RONDA " << _currentRound << " INICIADA!" << std::endl;
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

int		Game::getRound() const
{
	return (_currentRound);
}