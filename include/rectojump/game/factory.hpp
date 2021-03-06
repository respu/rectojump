//
// Copyright (c) 2013-2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef RJ_GAME_FACTORY_HPP
#define RJ_GAME_FACTORY_HPP


#include <memory>


namespace rj
{
	namespace factory
	{
		template<typename T>
		using eptr = std::shared_ptr<T>;

		template<typename T, typename... Args>
		eptr<T> create(Args&&... args)
		{return std::make_shared<T>(std::forward<Args>(args)...);}
	}
}



#endif // RJ_GAME_FACTORY_HPP
