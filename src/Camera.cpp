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

Camera::Camera(int width, int height) :
	_view{ 0, 0, (float)width, (float)height },
	_zoom(1.0f) {}

//	Centers the camera on the center of the map
void	Camera::update(int mapW, int mapH)
{
	float logicalW = this->_view.w;
	float logicalH = this->_view.h;

	this->_view.x = (mapW * PIXEL_SIZE - logicalW) / 2.0f;
	this->_view.y = (mapH * PIXEL_SIZE - logicalH) / 2.0f;
}

//	Centers the camera on a target (usually the player) and clamps it to the map boundaries
void	Camera::update(const SDL_FRect& target, int mapW, int mapH)
{
	//	Calculate the new camera position to center on the target
    float logicalW = this->_view.w;
    float logicalH = this->_view.h;

	//	Limit the camera to the map borders
	float maxMapX = static_cast<float>(mapW * PIXEL_SIZE);
	float maxMapY = static_cast<float>(mapH * PIXEL_SIZE);

	//	X axis
	if (maxMapX <= logicalW - (VOID_TILES * PIXEL_SIZE)) //	<- if map is small, center it on screen (considering void tiles)
		this->_view.x = -(logicalW - maxMapX) / 2.0f;
	else
	{
		//	else follow the player
		this->_view.x = (target.x + target.w / 2.0f) - (logicalW / 2.0f);

		//	but clamp to map edges (considering void tiles)
		if (this->_view.x < 0 - (VOID_TILES * PIXEL_SIZE))
			this->_view.x = 0 - (VOID_TILES * PIXEL_SIZE);
		if (this->_view.x > maxMapX - logicalW + (VOID_TILES * PIXEL_SIZE))
			this->_view.x = maxMapX - logicalW + (VOID_TILES * PIXEL_SIZE);
	}

	//	Same for Y axis
	if (maxMapY <= logicalH - (VOID_TILES * PIXEL_SIZE))
		this->_view.y = -(logicalH - maxMapY) / 2.0f;
	else
	{
		this->_view.y = (target.y + target.h / 2.0f) - (logicalH / 2.0f);

		if (this->_view.y < 0 - (VOID_TILES * PIXEL_SIZE))
			this->_view.y = 0 - (VOID_TILES * PIXEL_SIZE);
		if (this->_view.y > maxMapY - logicalH + (VOID_TILES * PIXEL_SIZE))
			this->_view.y = maxMapY - logicalH + (VOID_TILES * PIXEL_SIZE);
	}
}

//	Allows resizing the camera view (useful for window resizing)
void	Camera::resizeView(float newWidth, float newHeight)
{
	//	Calculate the new zoom level based on new resolution, target resolution and aspect ratio (considering FOV)
	float widthScale = newWidth / TARGET_WIDTH / 9.0f;
	float heightScale = newHeight / TARGET_HEIGHT / 16.0f;

	this->_zoom = std::min(widthScale, heightScale) * FOV;

	this->_view.w = newWidth / this->_zoom;
	this->_view.h = newHeight / this->_zoom;
}

//	Transforms a "world" coordinate to a "screen" coordinate
//	RETURN: A new SDL_FRect with the transformed coordinates
SDL_FRect	Camera::apply(const SDL_FRect& worldRect) const
{
	return {
		(worldRect.x - this->_view.x) * this->_zoom,
		(worldRect.y - this->_view.y) * this->_zoom,
		worldRect.w * this->_zoom,
		worldRect.h * this->_zoom
	};
}

void	Camera::setZoom(float zoom)
{
	if (zoom < 0)
	{
		if (Debug::state == true)
			std::cerr << B_RED << "[ ERROR ]" << NO_COLOR << "trying to set zoom to a negative value wtf -> " << BLUE << zoom << NO_COLOR << std::endl;
		return ;
	}
	_zoom = zoom;
}

//	Returns the current camera view (position and size)
const SDL_FRect& Camera::getView() const
{
	return this->_view;
}

//	Checks if a world-space rectangle is visible within the camera view
bool	Camera::isVisible(const SDL_FRect& worldRect) const
{
	return (SDL_HasRectIntersectionFloat(&this->_view, &worldRect));
}
