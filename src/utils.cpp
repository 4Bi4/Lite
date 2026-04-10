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

//	Prints the help page (possible arguments)
void	printUsage()
{
	std::cout << "usage: ./lite \"\"\n";
	std::cout << "              [--help]         - display this help page\n";
	std::cout << "              [--debug]        - displays debug information on the console\n";
	std::cout << "              [--no-vsync]     - turn off vsync on startup\n";
	std::cout << "              [--no-fps-limit] - disable FPS limit on startup\n" << std::endl;
}

//	Checks user argumets for flags (like "--debug")
//	exits the program if "--help" is passed
//	RETURN: 0 on success, 1 on error
int		checkArgs(char* argv[], Data& data)
{
	for (int i = 1; argv[i] ; i++)
	{
		if (std::string(argv[i]) == "--debug")
			Debug::state = true;
		else if (std::string(argv[i]) == "--help")
		{
			printUsage();
			exit(0);
		}
		else if (std::string(argv[i]) == "--no-fps-limit")
			data.setFpsLimit(0);
		else if (std::string(argv[i]) == "--no-vsync")
			data.setVsync(false);
		else
		{
			std::cerr << "unknown argument \"" << argv[i] << "\" try './lite --help'." << std::endl;
			return (1);
		}
	}
	return (0);
}
