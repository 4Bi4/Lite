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

// Signal tracking (volatile and atomic for async-signal-safe access)
volatile sig_atomic_t g_signalReceived = 0;
volatile sig_atomic_t g_signalNumber = 0;

void handle_signal(int sig)
{
	//	These are async-signal-safe operations
	g_signalNumber = sig;
	g_signalReceived = 1;
}

// THIS IS FOR THE TEXTURE MANAGER
//	|	|	|	|	|	|	|	|
//	V	V	V	V	V	V	V	V
std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;

//	Starts a new game
//	\returns
//	0 on success, 1 on error
int	initGame(Data& data)
{
	Game	game(data);
	data.setGame(&game);

	Map*	map	= game.getMap();
	Player*	player = game.getPlayer();

	//	Set the player's initial position to the center of the map
	float initialX = ((map->getWidth() * PIXEL_SIZE) / 2.0f);
	float initialY = ((map->getHeight() * PIXEL_SIZE) / 2.0f);
	player->setPosition(initialX, initialY);

	//	TESTING:
	//	Equip the player with a test weapon
	Weapon testWeapon(FIREBALL, player);
	player->setWeapon(&testWeapon);

	//	Call the main game loop
	data.setState(IN_GAME);
	if (game.gameLoop(data) != 0)
		return (1);

	//	Invalidate the pointer after the game
	data.setGame(nullptr);

	return (0);
}

int	main(int argc, char* argv[])
{
	Data	data;

	if (argc > 1 && checkArgs(argv, data) != 0)
		return (1);
	if (Debug::state == true)
		std::cout << BLUE << "debug mode" << NO_COLOR << " is" << B_GREEN << " on" << NO_COLOR << std::endl;

	//	Signal Handler (Ctrl+C)
	signal(SIGINT,  handle_signal);
	signal(SIGTERM, handle_signal);

	//	Init RNG seed
	std::srand(std::time(NULL));

	//	Initialize SDL and its subsystems
	if (Debug::state == true)
		std::cout << "initializing SDL..." << std::endl;
	if (initSDL(data) != 0)
		return (1);
	loadTextures(data);
	if (initSDLText(data) != 0)
		return (1);

	//	Start the game
	if (initGame(data) != 0)
		return (1);

	//	Close the program
	data.setState(CLOSING);
	TextureManager::Clean();

	return (0);
}
