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
	std::cout << "usage: ./lite [--debug]\n" << "              [--help]" << std::endl;
}

//	Checks user argumets for flags (like "--debug")
//	RETURN: 0 on success, 1 on error
int		checkArgs(char* argv[])
{
	for (int i = 1; argv[i] ; i++)
	{
		if (std::string(argv[i]) == "--debug")
			Debug::state = true;
		else if (std::string(argv[i]) == "--help")
			printUsage();
		else
		{
			std::cerr << "unknown argument \"" << argv[i] <<
				"\" try one of the following arguments:\n" << std::endl;
			printUsage();
			return (1);
		}
	}
	return (0);
}
