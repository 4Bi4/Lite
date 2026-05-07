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

#include <cmath>

Projectile::Projectile(ProjectileType type, ProjectileStats stats, EntityID target, ProjectileManager* manager) :
	_active(false),
	_flip(SDL_FLIP_NONE),
	_srcRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE},
	_destRect{ 0.0f, 0.0f, (float)PIXEL_SIZE, (float)PIXEL_SIZE},
	_type(type),
	_stats(stats),
	_target(target),
	_manager(manager)
{
	switch (type)
	{
		case (FIRE) :
			_texture = TextureManager::loadTexture(FIREBALL_TEXTURE, nullptr);
			break;
		default :
			_texture = TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, nullptr);
	}
}

Projectile::~Projectile() {}

void	Projectile::spawn(ProjectileID id, ProjectileType type, ProjectileStats stats, EntityID target, float x, float y)
{
	_id = id;
	_type = type;
	_stats = stats;
	_target = target;
	_active = true;
	_flip = SDL_FLIP_NONE;
	_destRect.w = (float)PIXEL_SIZE * _stats.sizeMultiplier;
	_destRect.h = (float)PIXEL_SIZE * _stats.sizeMultiplier;
	_destRect.x = x - (_destRect.w * 0.5f);
	_destRect.y = y - (_destRect.h * 0.5f);
	_srcRect.w = (float)PIXEL_SIZE * _stats.sizeMultiplier;
	_srcRect.h = (float)PIXEL_SIZE * _stats.sizeMultiplier;
	_hitbox = _destRect;


	switch (_type)
	{
		case (FIRE) :
			_texture = TextureManager::loadTexture(FIREBALL_TEXTURE, nullptr);
			_hitbox.w *= 0.2f; // Increase hitbox size based on sizeMultiplier
			_hitbox.h *= 0.2f;
			_hitbox.x = _destRect.x + (_destRect.w * 0.5f) - (_hitbox.w * 0.5f);
   			_hitbox.y = _destRect.y + (_destRect.h * 0.5f) - (_hitbox.h * 0.5f);

			break;
		default :
			_texture = TextureManager::loadTexture(DEFAULT_PLAYER_TEXTURE, nullptr);
	}
}

void	Projectile::despawn()
{
	_active = false;
}

SDL_FRect	Projectile::getRect() const
{
	return (_destRect);
}

SDL_FRect	Projectile::getHitbox() const
{
	return (_hitbox);
}

ProjectileID	Projectile::getID() const
{
	return (_id);
}

void	Projectile::update(float deltaTimeNS, Data& data)
{
	Enemy* target = EnemyManager::getEnemy(_target, *data.getGame());
	if (!target)
	{
		despawn();
		return;
	}

	const SDL_FRect& targetRect = target->getRect();
	const float targetCenterX = targetRect.x + (targetRect.w * 0.5f);
	const float targetCenterY = targetRect.y + (targetRect.h * 0.5f);
	const float projectileCenterX = _destRect.x + (_destRect.w * 0.5f);
	const float projectileCenterY = _destRect.y + (_destRect.h * 0.5f);

	float dirX = targetCenterX - projectileCenterX;
	float dirY = targetCenterY - projectileCenterY;
	float length = std::sqrt((dirX * dirX) + (dirY * dirY));

	if (length > 0.0f)
	{
		dirX /= length;
		dirY /= length;

		_angle = (std::atan2(dirY, dirX) * 180.0f / PI);
	}

	_destRect.x += dirX * _stats.speed * deltaTimeNS / 1000000000.0f;
	_destRect.y += dirY * _stats.speed * deltaTimeNS / 1000000000.0f;

	//	Update hitbox position
	// Update hitbox position (keeping it centered)
	float offsetX = (_destRect.w - _hitbox.w) * 0.5f;
	float offsetY = (_destRect.h - _hitbox.h) * 0.5f;

	_hitbox.x = _destRect.x + offsetX;
	_hitbox.y = _destRect.y + offsetY;	
}

void	Projectile::render(Data& data)
{
	//	Get the position relative to the camera
	SDL_FRect screenRect = data.getGame()->getCamera()->apply(_destRect);

	if (!_texture)
	{
		if (Debug::state == true)
			std::cerr << B_RED << "[ ERROR ] projectile without texture!" << NO_COLOR << std::endl;
		return ;
	}
	//	Render the projectile to the screen
	SDL_RenderTextureRotated(data.getRenderer(), _texture, &_srcRect, &screenRect, 0.0, NULL, _flip);
}

ProjectileType	Projectile::getType() const
{
	return (this->_type);
}

ProjectileStats	Projectile::getStats() const
{
	return (this->_stats);
}

const EntityID			Projectile::getTarget() const
{
	return (this->_target);
}

const ProjectileManager*	Projectile::getManager() const
{
	return (_manager);
}

SDL_Texture*			Projectile::getTexture() const
{
	return (this->_texture);
}

int	Projectile::getPierceCount() const
{
	return (_stats.pierceCount);
}

bool	Projectile::isActive() const
{
	return (this->_active);
}

void	Projectile::setTexture(SDL_Texture* texture)
{
	this->_texture = texture;
}

void	Projectile::consumePierce()
{
	if (_stats.pierceCount > 0)
		_stats.pierceCount--;
}
