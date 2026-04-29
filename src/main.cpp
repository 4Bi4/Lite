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

//	Runs the game logic (movement, entities, etc...)
//	RETURN: 0 on success, 1 on error
int	gameLogic(Data& data, Uint64 deltaTime)
{
	if (!data.player)
	{
		std::cerr << B_RED << "Error: Player not initialized!" << NO_COLOR << std::endl;
		return (1);
	}
	data.player->update(deltaTime, data);

	for (auto& Enemy : data.enemies)
		Enemy.update(deltaTime, data);

	data.camera->update(
		data.player->getRect(), 
		data.map->getWidth(), 
		data.map->getHeight()
	);

	return (0);
}

//	Renders the game (background, map, entities, etc...)
void	renderLogic(Data& data)
{
	//	--- Background ---
	makeBGRainbow(data);
	data.map->drawMap(data.getRenderer(), data.camera);

	//	--- Foreground ---
	for (auto& Enemy : data.enemies)
		Enemy.render(data);

	data.player->render(data);
}

//	Main loop of the engine
//	RETURN: 0 on success, 1 on error
int	gameLoop(Data& data)
{
	SDL_Event	event;
	Uint64		lastFrame = SDL_GetTicksNS();
	long long	frameCount = 0;
	long long	totalTime = 0;

	//	Check all the modules
	if (!data.player || !data.map || !data.camera)
	{
		std::cerr << B_RED << "Error: Not all game modules initialized!" << NO_COLOR << std::endl;
		return (1);
	}

	//	Set the player's initial position to the center of the map
	float initialX = ((data.map->getWidth() * PIXEL_SIZE) / 2.0f);
	float initialY = ((data.map->getHeight() * PIXEL_SIZE) / 2.0f);
	data.player->setPosition(initialX, initialY);

	//	Update the State Machine
	data.setState(IN_GAME);

	//	TESTING:
	//  Esto es cutrisimo hay que quitarlo despues!!!

	SDL_Texture* targetTexture = TextureManager::loadTexture(DEBUG_ENEMY_TEXTURE, data.getRenderer());

	while (data.isRunning())
	{
		Uint64	currentFrame = SDL_GetTicksNS();
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
		if (gameLogic(data, deltaTime) != 0)
			return (1);
	
		//	TESTING:
		if (Debug::state == true)
		{
			Enemy* newTarget = data.player->getClosestEnemy(data);
			Enemy* oldTarget = data.player->getTarget();
			if (newTarget != oldTarget)
			{
				data.player->setTarget(newTarget);
				newTarget->setTexture(targetTexture);
				if (oldTarget)
					oldTarget->setTexture(TextureManager::getTexture(DEFAULT_ENEMY_TEXTURE));
			}
		}

		//	Render stuff here
		renderLogic(data);

		//	Frame limiting (if vsync is disabled)
		Uint64	targetNS = (Uint64)data.getTargetFrameTime() * 1000000;
		Uint64	frameWorkTime = SDL_GetTicksNS() - currentFrame;

		if (data.getFpsLimit() > 0 && frameWorkTime < targetNS)
		{
			SDL_DelayNS(targetNS - frameWorkTime);
		}
		SDL_RenderPresent(data.getRenderer());
	}

	//	Update the State Machine
	data.setState(POSTGAME);

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

int	main(int argc, char* argv[])
{
	Data	data;

	if (argc > 1 && checkArgs(argv, data) != 0)
		return (1);
	if (Debug::state == true)
		std::cout << BLUE << "debug mode" << NO_COLOR << " is" << B_GREEN << " on" << NO_COLOR << std::endl;

	if (Debug::state == true)
		std::cout << "initializing SDL..." << std::endl;

	if (initSDL(data) != 0)
		return (1);


	Player player(TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, data.getRenderer()));
	data.player = &player;
	
	//	TESTING:
	//	Create an enemy and give it a random position
	Enemy joe(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()));
	joe.setPosition(1000.0f, 1000.0f);
	data.enemies.push_back(joe);

	Enemy joe2(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()));
	joe2.setPosition(100.0f, 100.0f);
	data.enemies.push_back(joe2);

	Enemy joe3(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()));
	joe3.setPosition(1500.0f, 100.0f);
	data.enemies.push_back(joe3);

	Enemy joe4(TextureManager::loadTexture(DEFAULT_ENEMY_TEXTURE, data.getRenderer()));
	joe4.setPosition(1500.0f, 1200.0f);
	data.enemies.push_back(joe4);

	Map map(data.getRenderer(), MAP_HEIGHT, MAP_WIDTH);
	data.map = &map;

	Camera camera(data.getHres(), data.getVres());
	data.camera = &camera;

	//	Start the game
	gameLoop(data);

	//	Close the program
	data.setState(CLOSING);
	TextureManager::Clean();

	return (0);
}
