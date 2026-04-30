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

	void	update(float deltaTimeNS, Data& data);

	void	render(Data& data);

	void	nextRound();

	void	togglePause();

	void	gameOver();

	bool	isPaused() const;
	bool	isGameOver() const;
	float	getRemainingTime() const;
	int		getRound() const;

	EnemyManager	enemyManager;
	Player			player;
	Camera			camera;
	Map				map;

private:
	float	_roundTimer;
	float	_roundDuration;
	int		_currentRound;
	bool	_isPaused;
	bool	_isGameOver;
};
