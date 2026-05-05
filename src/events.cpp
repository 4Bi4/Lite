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

//	Handles the press F to toggle fullscreen
void	handleFullscreenEvent(Data& data)
{
	bool	newState = !data.isFullscreen();

	data.setFullscreen(newState);

	if (SDL_SetWindowFullscreen(data.getWindow(), newState) != true)
	{
		std::cerr << "SDL_SetWindowFullscreen: " << SDL_GetError() << "\n";
		data.setFullscreen(!newState); // Revert state on failure
	}	
}

//	Handles window resize events to update camera view
void	handleWindowResizeEvent(Data& data)
{
	int	newW;
	int	newH;

	SDL_GetRenderOutputSize(data.getRenderer(), &newW, &newH);
	
	data.setHres(newW);
	data.setVres(newH);

	data.getGame()->getCamera()->resizeView((float)newW, (float)newH);
}

//	Handles gamepad connection and disconnection events
int	handleGamepadEvent(Data& data, SDL_Event& event)
{
	if (event.type == SDL_EVENT_GAMEPAD_ADDED)
	{
		//	We support the first controller connected (for now)
		if (!data.getGamepad())
		{
			// event.gdevice.which = new device ID
			SDL_Gamepad* newGamepad = SDL_OpenGamepad(event.gdevice.which);
			if (!newGamepad)
			{
				std::cerr << "SDL_OpenGamepad: " << SDL_GetError() << "\n";
				return (1);
			}
			data.setGamepad(newGamepad);
			if (Debug::state == true)
				std::cout << B_BLUE << "New controller connected!" << NO_COLOR << std::endl;
		}
	}

	if (event.type == SDL_EVENT_GAMEPAD_REMOVED)
	{
		if (data.getGamepad())
		{
			//	Close gamepad (prevents memory leak)
			SDL_CloseGamepad(data.getGamepad());
			data.setGamepad(nullptr);
			if (Debug::state == true)
				std::cout << "Controller disconnected." << std::endl;
		}
	}
	return (0);
}

//	Handles events (keyboard, mouse, gamepad, window events, etc...)
//	RETURN: 0 on success, 1 on error
int handleEvents(Data& data, SDL_Event& event)
{
	//	Handle events
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			data.setRunning(false);
			data.getGame()->gameOver();
		}

		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			//	Handle fullscreen toggle
			if (event.key.key == SDLK_F && !event.key.repeat)
				handleFullscreenEvent(data);
		}

		//	Handle window resize events to update camera view
		if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED || event.type == SDL_EVENT_WINDOW_RESIZED) 
			handleWindowResizeEvent(data);

		//	Gamepad/Controller management
		if (event.type == SDL_EVENT_GAMEPAD_ADDED || event.type == SDL_EVENT_GAMEPAD_REMOVED)
		{
			if (handleGamepadEvent(data, event) != 0)
				std::cerr << B_RED << "!!Error adding a new gamepad!!" << NO_COLOR << std::endl;
		}
	}
	return (0);
}
