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

int main(int argc, char* argv[])
{
	if (argc > 1 && checkArgs(argv) != 0)
		return (1);
	if (Debug::state == true)
		std::cout << BLUE << "debug mode" << NO_COLOR << " is" << B_GREEN << " on" << NO_COLOR << std::endl;

	Data	data;

	if (Debug::state == true)
		std::cout << "initializing SDL..." << std::endl;
	if (initSDL() != 0)
		return (1);
	if (Debug::state == true)
	{
		std::cout << "SDL initialized:\n" << "\n";
		std::cout << "  horizontal resolution: " << data.getHres() << "\n";
		std::cout << "  vertical resolution: " << data.getVres() << "\n";
		std::cout << "  FPS limit: " << data.getFpsLimit() << std::endl;
	}
	return (0);
}
