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

#include "../include/Camera.hpp"

Camera::Camera(int width, int height) :
	_view{ 0, 0, (float)width, (float)height } {}

//	Centers the camera on a target (usually the player) and clamps it to the map boundaries
void	Camera::update(const SDL_FRect& target, int mapW, int mapH)
{
	// Calculate the new camera position to center on the target
    float logicalW = this->_view.w;
    float logicalH = this->_view.h;

	// Limit the camera to the map borders
	float maxMapX = static_cast<float>(mapW * PIXEL_SIZE);
	float maxMapY = static_cast<float>(mapH * PIXEL_SIZE);

	//	X axis
	if (maxMapX <= logicalW) //	<- if map is small, center it on screen
		this->_view.x = -(logicalW - maxMapX) / 2.0f;
	else
	{
		//	else follow the player
		this->_view.x = (target.x + target.w / 2.0f) - (logicalW / 2.0f);
		//	but clamp to map edges
		if (this->_view.x < 0) this->_view.x = 0;
		if (this->_view.x > maxMapX - logicalW) this->_view.x = maxMapX - logicalW;
	}

	//	Same for Y axis
	if (maxMapY <= logicalH)
		this->_view.y = -(logicalH - maxMapY) / 2.0f;
	else
	{
		this->_view.y = (target.y + target.h / 2.0f) - (logicalH / 2.0f);
		if (this->_view.y < 0) this->_view.y = 0;
		if (this->_view.y > maxMapY - logicalH) this->_view.y = maxMapY - logicalH;
	}
}

//	Allows resizing the camera view (useful for window resizing)
void	Camera::resizeView(float newWidth, float newHeight)
{
	this->_view.w = newWidth;
	this->_view.h = newHeight;
}

//	Transforms a "world" coordinate to a "screen" coordinate
//	RETURN: A new SDL_FRect with the transformed coordinates
SDL_FRect	Camera::apply(const SDL_FRect& worldRect) const
{
	return
	{
		worldRect.x - this->_view.x,
		worldRect.y - this->_view.y,
		worldRect.w,
		worldRect.h
	};
}
