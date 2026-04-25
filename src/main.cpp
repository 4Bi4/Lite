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

int	gameLogic(Data& data, Uint64 deltaTime)
{
	if (!data._player)
	{
		std::cerr << B_RED << "Error: Player not initialized!" << NO_COLOR << std::endl;
		return (1);
	}
	data._player->update(deltaTime, data);

	data._camera->update(
        data._player->getRect(), 
        data._map->getWidth(), 
        data._map->getHeight()
    );

	return (0);
}

void	renderLogic(Data& data)
{
	//	Background
	makeBGRainbow(data);
	data._map->drawMap(data.getRenderer(), data._camera);

	//	Foreground
	data._player->render(data);
}

//	Main loop of the engine
//	RETURN: 0 on success, 1 on error
int	mainLoop(Data& data)
{
	SDL_Event	event;
	Uint64		lastFrame = SDL_GetTicksNS();
	long long	frameCount = 0;
	long long	totalTime = 0;

	//	Check all the modules
	if (!data._player || !data._map || !data._camera)
	{
		std::cerr << B_RED << "Error: Not all game modules initialized!" << NO_COLOR << std::endl;
		return (1);
	}

	//	Set the player's initial position to the center of the map
	float initialX = ((data._map->getWidth() * PIXEL_SIZE) / 2.0f) - (PIXEL_SIZE / 2.0f);
	float initialY = ((data._map->getHeight() * PIXEL_SIZE) / 2.0f) - (PIXEL_SIZE / 2.0f);
	data._player->setPosition(initialX, initialY);

	while (data.isRunning())
	{
		Uint64	currentFrame = SDL_GetTicksNS();
		Uint64	deltaTime = currentFrame - lastFrame;

		lastFrame = currentFrame;

		if (Debug::state == true)
		{
			totalTime += deltaTime;
			frameCount++;
		}

		//	Clear the screen before rendering
		SDL_RenderClear(data.getRenderer());
		
		//	Handle events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				data.setRunning(false);

			//	Handle window resize events to update camera view
			if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED || event.type == SDL_EVENT_WINDOW_RESIZED) 
			{
				int	newW;
				int	newH;

				SDL_GetRenderOutputSize(data.getRenderer(), &newW, &newH);
				
				data.setHres(newW);
				data.setVres(newH);

				if (data._camera)
					data._camera->resizeView((float)newW, (float)newH);
			}
			// Future: Handle keyboard/mouse events here
		}

		//Move game logic to a separate function for better organization
		if (gameLogic(data, deltaTime) != 0)
			return (1);
	
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

	Player player(TextureManager::loadTexture("./resources/textures/player/error.png", data.getRenderer()));
	data._player = &player;
	
	Map map(data.getRenderer(), MAP_HEIGHT, MAP_WIDTH);
	data._map = &map;

	Camera camera(data.getHres(), data.getVres());
	data._camera = &camera;

	mainLoop(data);

	TextureManager::Clean();
	return (0);
}
