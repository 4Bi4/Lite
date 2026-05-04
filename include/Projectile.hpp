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

class ProjectileManager;

class Projectile
{
public:
	Projectile(ProjectileType type, ProjectileStats stats, EntityID target, ProjectileManager* manager);
	~Projectile();

	void	spawn(ProjectileID id, ProjectileType type, ProjectileStats stats, EntityID target, float x, float y);
	void	despawn();

	void	render(Data& data);
	void	update(float deltaTimeNS, Data& data);

	SDL_FRect					getRect() const;
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

	ProjectileType				_type;
	ProjectileStats				_stats;
	EntityID					_target;//	Enemy target
	const ProjectileManager*	_manager;
};
