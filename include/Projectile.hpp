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

#pragma once

#include "lite_common.hpp"

class Game;
class ProjectileManager;

class Projectile
{
public:
	Projectile(ProjectileType type, ProjectileStats stats, EntityID target, ProjectileManager* manager);
	~Projectile();

	void	spawn(ProjectileID id, ProjectileType type, ProjectileStats stats, EntityID target, float x, float y);
	void	despawn();

	void	render(Data& data);
	void	update(float deltaTimeNS, Game& game);

	SDL_FRect					getRect() const;
	SDL_FRect					getHitbox() const;
	ProjectileID				getID() const;
	ProjectileType				getType() const;
	ProjectileStats				getStats() const;
	const EntityID				getTarget() const;
	const ProjectileManager*	getManager() const;
	SDL_Texture*				getTexture() const;
	int						getPierceCount() const;

	bool	isActive() const;
	void	setTexture(SDL_Texture* texture);
	void	consumePierce();

protected:
	bool						_active;
	ProjectileID				_id;
	SDL_FlipMode				_flip;
	SDL_Texture*				_texture;
	SDL_FRect					_srcRect;
	SDL_FRect					_destRect;
	SDL_FRect					_hitbox;
	float						_angle;
	float						_dirX;
	float						_dirY;

	ProjectileType				_type;
	ProjectileStats				_stats;
	EntityID					_target;//	Enemy target
	const ProjectileManager*	_manager;
};
