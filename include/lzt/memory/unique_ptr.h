
#ifndef LZT_UNIQUE_PTR_H
#define LZT_UNIQUE_PTR_H

#include <memory>
#include <utility>

namespace lzt {
	template<typename T, typename Deleter = std::default_delete<T>>
	class unique_ptr {
	public:
		using pointer = T*;
		using element_type = T;
		using deleter_type = Deleter;
	public:
		explicit unique_ptr(pointer ptr = nullptr) noexcept : _pointer(ptr) {}

		unique_ptr(const unique_ptr&) = delete;

		unique_ptr& operator=(const unique_ptr&) = delete;

		unique_ptr(unique_ptr&& other) noexcept
			: _pointer(other._pointer), _deleter(std::move(other._deleter)) {

			other._pointer = nullptr;
		}

		unique_ptr& operator=(unique_ptr&& other) noexcept {
			if (this != &other) {
				reset();
				_pointer = other._pointer;
				_deleter = std::move(other._deleter);
				other._pointer = nullptr;
			}
			return *this;
		}

		~unique_ptr() {
			reset();
		}

		pointer release() noexcept {
			pointer oldPointer = _pointer;
			_pointer = nullptr;
			return oldPointer;
		}

		void reset(pointer ptr = nullptr) noexcept {
			if (_pointer)
				_deleter(_pointer);
			_pointer = ptr;
		}

		void swap(unique_ptr& other) noexcept {
			std::swap(_pointer, other._pointer);
			std::swap(_deleter, other._deleter);
		}

		pointer get() const noexcept {
			return _pointer;
		}

		deleter_type& get_deleter() noexcept {
			return _deleter;
		}

		const deleter_type& get_deleter() const noexcept {
			return _deleter;
		}

		explicit operator bool() const noexcept {
			return _pointer != nullptr;
		}

		T& operator*() const noexcept {
			return *_pointer;
		}

		pointer operator->() const noexcept {
			return _pointer;
		}
	private:
		pointer _pointer = nullptr;
		deleter_type _deleter;
	};

	template<typename T, typename Deleter>
	class unique_ptr<T[], Deleter> {
	public:
		using pointer = T*;
		using element_type = T;
		using deleter_type = Deleter;
	public:
		explicit unique_ptr(pointer ptr = nullptr) noexcept : _pointer(ptr) {}

		~unique_ptr() {
			reset();
		}

		unique_ptr(const unique_ptr&) = delete;

		unique_ptr& operator=(const unique_ptr&) = delete;

		unique_ptr(unique_ptr&& other) noexcept
			: _pointer(other._pointer), _deleter(std::move(other._deleter)) {

			other._pointer = nullptr;
		}

		unique_ptr& operator=(unique_ptr&& other) noexcept {
			if (this != &other) {
				reset();
				_pointer = other._pointer;
				_deleter = std::move(other._deleter);
				other._pointer = nullptr;
			}
			return *this;
		}

		pointer release() noexcept {
			pointer oldPointer = _pointer;
			_pointer = nullptr;
			return oldPointer;
		}

		void reset(pointer ptr = nullptr) noexcept {
			if (_pointer)
				_deleter(_pointer);
			_pointer = ptr;
		}

		void swap(unique_ptr& other) noexcept {
			std::swap(_pointer, other._pointer);
			std::swap(_deleter, other._deleter);
		}

		pointer get() const noexcept {
			return _pointer;
		}

		deleter_type& get_deleter() noexcept {
			return _deleter;
		}

		const deleter_type& get_deleter() const noexcept {
			return _deleter;
		}

		T& operator*() const = delete;

		pointer operator->() const = delete;

		T& operator[](size_t index) const {
			return _pointer[index];
		}
	private:
		pointer _pointer = nullptr;
		deleter_type _deleter;
	};
}

#endif // LZT_UNIQUE_PTR_H