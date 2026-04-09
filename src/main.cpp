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

//	Main loop of the engine
//	RETURN: 0 on success, 1 on error
int	mainLoop(Data& data)
{
	SDL_Event	event;
	Uint64		lastFrame = SDL_GetTicksNS();
	long long	frameCount = 0;
	long long	totalTime = 0;

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

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				data.setRunning(false);

			// Future: Handle keyboard/mouse events here
		}
		// Future: Update game state and render here
		Uint64	targetNS = (Uint64)data.getTargetFrameTime() * 1000000;
		Uint64	frameWorkTime = SDL_GetTicksNS() - currentFrame;

		if (data.getFpsLimit() > 0 && frameWorkTime < targetNS)
		{
			SDL_DelayNS(targetNS - frameWorkTime);
		}
	}

	// DEBUG OUTPUT
	if (Debug::state == true && frameCount > 0)
	{
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

	mainLoop(data);

	return (0);
}
