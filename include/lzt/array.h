
#ifndef LZT_ARRAY_H
#define LZT_ARRAY_H

#include <algorithm>
#include <stdexcept>
#include <initializer_list>

#include <lzt/iterator/random_access_iterator.h>

namespace lzt {
	template<typename T, size_t Size>
	class array {
	public:
		using value_type = T;
		using size_type = size_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using const_iterator = const_random_access_iterator<T>;
		using iterator = random_access_iterator<T>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	public:
		array() {};

		constexpr array(std::initializer_list<T> init) {
			if (Size < init.size())
				throw std::out_of_range("too many elements in initializer_list");

			std::copy(init.begin(), init.end(), begin());

			if (init.size() < Size)
				std::fill(begin() + init.size(), end(), T{});
		}

		constexpr void fill(const T& value) noexcept {
			for (size_t i = 0; i < Size; ++i) {
				_data[i] = value;
			}
		}

		constexpr void swap(array& other) noexcept {
			for (size_t i = 0; i < Size; ++i) {
				std::swap(_data[i], other._data[i]);
			}
		}

		friend constexpr void swap(array& lhs, array& rhs) noexcept {
			lhs.swap(rhs);
		}

		constexpr iterator begin() noexcept {
			return iterator(_data);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(_data);
		}

		constexpr iterator end() noexcept {
			return iterator(_data + Size);
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(_data + Size);
		}

		constexpr reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		constexpr const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		constexpr reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		constexpr const_iterator cbegin() const noexcept {
			return begin();
		}

		constexpr const_iterator cend() const noexcept {
			return end();
		}

		constexpr const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		constexpr const_reverse_iterator crend() const noexcept {
			return rend();
		}

		constexpr bool empty() const noexcept {
			return Size == 0;
		}

		constexpr size_t size() const noexcept {
			return Size;
		}

		constexpr size_t max_size() const noexcept {
			return Size;
		}

		constexpr reference at(const size_t index) {
			if (Size <= index)
				throw std::out_of_range("index is out of range");
			return _data[index];
		}

		constexpr const_reference at(const size_t index) const {
			if (Size <= index)
				throw std::out_of_range("index is out of range");
			return _data[index];
		}

		constexpr reference operator[](const size_t index) noexcept {
			return _data[index];
		}

		constexpr const_reference operator[](const size_t index) const noexcept {
			return _data[index];
		}

		constexpr reference front() noexcept {
			return _data[0];
		}

		constexpr const_reference front() const noexcept {
			return _data[0];
		}

		constexpr reference back() noexcept {
			return _data[Size - 1];
		}

		constexpr const_reference back() const noexcept {
			return _data[Size - 1];
		}

		constexpr pointer data() noexcept {
			return _data;
		}

		constexpr const_pointer data() const noexcept {
			return _data;
		}
	private:
		value_type _data[Size];
	};
}


#endif // LZT_ARRAY_H