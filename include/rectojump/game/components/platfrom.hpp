//
// Copyright (c) 2013 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_COMPONENTS_PLATFORM_HPP
#define RJ_GAME_COMPONENTS_PLATFORM_HPP


#include <rectojump/game/entity_rect.hpp>


namespace rj
{
	class platform : public entity_rect
	{
	public:
		platform(const sf::Vector2f& pos, const sf::Vector2f& size = {20.f, 20.f}) :
			entity_rect{pos, size, {-0.3f, 0.f}}
		{m_render_object.setOrigin(size.x / 2, size.y / 2);}

		~platform() = default;

	private:
		void update(dur duration) override
		{m_render_object.move(m_velocity * duration);}
	};
}



#endif // RJ_GAME_COMPONENTS_PLATFORM_HPP
