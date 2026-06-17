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

class Game
{
public:
	Game(Data& data);
	~Game();

	int		gameLoop(Data& data);

	void	update(float deltaTimeNS, Data& data);
	void	render(Data& data);

	void	nextRound();
	void	togglePause();
	void	gameOver();

	bool			isPaused() const;
	bool			isGameOver() const;
	float			getRemainingTime() const;
	unsigned int	getRound() const;

	//	Getters
	ProjectileManager*	getProjectileManager();
	EnemyManager*		getEnemyManager();
	Player*				getPlayer();
	Camera*				getCamera();
	Map*				getMap();
	RenderQueue*		getRenderQueue();

	//	Setters
	void			setPlayer(const Player& player);
	void			setCamera(const Camera& camera);
	void			setMap(const Map& map);

	//	Debug
	bool			displaySignalDebugInfo(long long frameCount, long long totalTime, Uint64 deltaTime);

private:
	ProjectileManager	_projectileManager;
	EnemyManager		_enemyManager;
	Player				_player;
	Camera				_camera;
	Map					_map;
	RenderQueue			_renderQueue;

	float			_roundTimer;
	float			_roundDuration;
	unsigned int	_currentRound;
	bool			_isPaused;
	bool			_isGameOver;
};
