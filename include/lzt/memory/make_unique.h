
#ifndef LZT_MAKE_UNIQUE_H
#define LZT_MAKE_UNIQUE_H

#include <utility>
#include <type_traits>
#include "unique_ptr.h"

namespace lzt {
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_array_v<T>, unique_ptr<T>>
		make_unique(Args&&... args) {
		return unique_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template<typename T>
	std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> == 0, unique_ptr<T>>
		make_unique(size_t size) {
		return unique_ptr<T[]>(new T[size]());
	}

	template<typename T, typename... Args>
	std::enable_if_t<std::is_array_v<T>&& std::extent_v<T> != 0, void>
		make_unique(Args&&...) = delete;
}

#endif // LZT_MAKE_UNIQUE_H