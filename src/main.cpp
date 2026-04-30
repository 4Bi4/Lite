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

// THIS IS FOR THE TEXTURE MANAGER
//	|	|	|	|	|	|	|	|
//	V	V	V	V	V	V	V	V

// Static member initialization
std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;

//	Main loop of the engine
//	RETURN: 0 on success, 1 on error
int	gameLoop(Data& data)
{
	SDL_Event	event;
	Uint64		lastFrame = SDL_GetTicksNS();
	long long	frameCount = 0;
	long long	totalTime = 0;
	Game		game(data);

	data.setGame(&game);

	//	Set the player's initial position to the center of the map
	float initialX = ((game.map.getWidth() * PIXEL_SIZE) / 2.0f);
	float initialY = ((game.map.getHeight() * PIXEL_SIZE) / 2.0f);
	game.player.setPosition(initialX, initialY);

	//	TESTING:
	//	Equip the player with a test weapon
	Weapon testWeapon("Fireball!!", 10, 150, 1000);
	game.player.setWeapon(&testWeapon);

	//	Update the State Machine
	data.setState(IN_GAME);

	while (!game.isGameOver())
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
		game.update(deltaTime, data);
	
		//	Render stuff here
		game.render(data);

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

	//	TODO:
	//	Make a proper loading screen and load assets there
	//	for now we just load a texture to test the Texture Manager
	TextureManager::loadTexture(DEBUG_ENEMY_TEXTURE, data.getRenderer());

	//	Start the game
	gameLoop(data);

	//	Close the program
	data.setState(CLOSING);
	TextureManager::Clean();

	return (0);
}
