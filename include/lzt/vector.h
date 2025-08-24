
#ifndef LZT_VECTOR_H
#define LZT_VECTOR_H

#include <initializer_list>
#include <algorithm>

namespace lzt {
	template<typename vector>
	class vector_const_iterator {
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename vector::value_type;
		using difference_type = typename vector::difference_type;
		using pointer = typename vector::const_pointer;
		using reference = typename vector::const_reference;
	public:
		constexpr vector_const_iterator() noexcept = default;

		constexpr explicit vector_const_iterator(pointer ptr) : _pointer(ptr) {}

		constexpr reference operator*() const noexcept {
			return *_pointer;
		}

		constexpr pointer operator->() const noexcept {
			return _pointer;
		}

		constexpr vector_const_iterator& operator++() noexcept {
			++_pointer;
			return *this;
		}

		constexpr vector_const_iterator operator++(int) noexcept {
			vector_const_iterator temp = *this;
			++(*this);
			return temp;
		}

		constexpr vector_const_iterator& operator--() noexcept {
			--_pointer;
			return *this;
		}

		constexpr vector_const_iterator operator--(int) noexcept {
			vector_const_iterator temp = *this;
			--(*this);
			return temp;
		}

		constexpr vector_const_iterator& operator+=(const difference_type offset) noexcept {
			_pointer += offset;
			return *this;
		}

		constexpr vector_const_iterator operator+(const difference_type offset) const noexcept {
			return vector_const_iterator(_pointer + offset);
		}

		friend constexpr vector_const_iterator operator+(
			const difference_type offset, vector_const_iterator next) noexcept {
			next += offset;
			return next;
		}

		constexpr vector_const_iterator& operator-=(const difference_type offset) noexcept {
			_pointer -= offset;
			return *this;
		}

		constexpr vector_const_iterator operator-(const difference_type offset) const noexcept {
			return vector_const_iterator(_pointer - offset);
		}

		constexpr difference_type operator-(const vector_const_iterator& other) const noexcept {
			return static_cast<difference_type>(_pointer - other._pointer);
		}

		constexpr reference operator[](const difference_type offset) const noexcept {
			return *(*this + offset);
		}

		constexpr bool operator==(const vector_const_iterator& other) const noexcept {
			return _pointer == other._pointer;
		}

		constexpr bool operator!=(const vector_const_iterator& other) const noexcept {
			return !(*this == other);
		}

		constexpr bool operator<(const vector_const_iterator& other) const noexcept {
			return _pointer < other._pointer;
		}

		constexpr bool operator>(const vector_const_iterator& other) const noexcept {
			return other < *this;
		}

		constexpr bool operator<=(const vector_const_iterator& other) const noexcept {
			return !(other < *this);
		}

		constexpr bool operator>=(const vector_const_iterator& other) const noexcept {
			return !(*this < other);
		}
	protected:
		pointer _pointer;
	};

	template<typename vector>
	class vector_iterator : public vector_const_iterator<vector> {
	public:
		using myBase = vector_const_iterator<vector>;

		using iterator_category = std::random_access_iterator_tag;
		using value_type = typename vector::value_type;
		using difference_type = typename vector::difference_type;
		using pointer = typename vector::pointer;
		using reference = typename vector::reference;
	public:
		using myBase::myBase;

		constexpr reference operator*() const noexcept {
			return const_cast<reference>(myBase::operator*());
		}

		constexpr pointer operator->() const noexcept {
			return const_cast<pointer>(myBase::operator->());
		}

		constexpr vector_iterator& operator++() noexcept {
			myBase::operator++();
			return *this;
		}

		constexpr vector_iterator operator++(int) noexcept {
			vector_iterator _Tmp = *this;
			myBase::operator++();
			return _Tmp;
		}

		constexpr vector_iterator& operator--() noexcept {
			myBase::operator--();
			return *this;
		}

		constexpr vector_iterator operator--(int) noexcept {
			vector_iterator _Tmp = *this;
			myBase::operator--();
			return _Tmp;
		}

		constexpr vector_iterator& operator+=(const difference_type offset) noexcept {
			myBase::operator+=(offset);
			return *this;
		}

		constexpr vector_iterator operator+(const difference_type offset) const noexcept {
			return vector_iterator(this->_pointer + offset);
		}

		friend constexpr vector_iterator operator+(
			const difference_type offset, vector_iterator next) noexcept {
			next += offset;
			return next;
		}

		constexpr vector_iterator& operator-=(const difference_type offset) noexcept {
			myBase::operator-=(offset);
			return *this;
		}

		using myBase::operator-;

		constexpr vector_iterator operator-(const difference_type offset) const noexcept {
			return vector_iterator(this->_pointer - offset);
		}

		constexpr reference operator[](const difference_type offset) const noexcept {
			return const_cast<reference>(myBase::operator[](offset));
		}
	};

	template <typename T>
	class vector {
	public:
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using const_iterator = vector_const_iterator<vector>;
		using iterator = vector_iterator<vector>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	public:
		vector() : _data(nullptr), _size(0), _capacity(0) {}

		vector(std::initializer_list<T> init) : _size(init.size()), _capacity(init.size()) {
			_data = (T*)::operator new(_capacity * sizeof(T));
			size_type i = 0;
			for (const auto& el : init) {
				new (&_data[i++]) T(std::move(el));
			}
		}

		vector(const vector& other)
			: _data((T*)::operator new(other._capacity * sizeof(T))), _size(other._size), _capacity(other._capacity) {

			for (size_t i = 0; i < _size; i++) {
				new (&_data[i]) T(other._data[i]);
			}
		}

		vector& operator=(vector other) {
			swap(other);
			return *this;
		}

		vector(vector&& other) noexcept
			: _data(other._data), _size(other._size), _capacity(other._capacity) {

			other._data = nullptr;
			other._size = 0;
			other._capacity = 0;
		}

		~vector() {
			clear();
			::operator delete(_data);
		}

		constexpr reference at(const size_type index) {
			if (_size <= index) {
				throw std::out_of_range("index is out of range");
			}
			return _data[index];
		}

		constexpr const_reference at(const size_type index) const {
			if (_size <= index) {
				throw std::out_of_range("index is out of range");
			}
			return _data[index];
		}

		constexpr reference operator[](const size_type index) noexcept {
			return _data[index];
		}

		constexpr const_reference operator[](const size_type index) const noexcept {
			return _data[index];
		}

		constexpr reference front() noexcept {
			return _data[0];
		}

		constexpr const_reference front() const noexcept {
			return _data[0];
		}

		constexpr reference back() noexcept {
			return _data[_size - 1];
		}

		constexpr const_reference back() const noexcept {
			return _data[_size - 1];
		}

		constexpr pointer data() noexcept {
			return _data;
		}

		constexpr const_pointer data() const noexcept {
			return _data;
		}

		constexpr iterator begin() noexcept {
			return iterator(_data);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(_data);
		}

		constexpr iterator end() noexcept {
			return iterator(_data + _size);
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(_data + _size);
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
			return _size == 0;
		}

		constexpr size_type size() const noexcept {
			return _size;
		}

		constexpr size_type max_size() const noexcept {
			return std::numeric_limits<size_type>::max() / sizeof(T);
		}

		constexpr void reserve(size_t newCapacity) {
			if (_capacity < newCapacity)
				_reallocation(newCapacity);
		}

		constexpr size_type capacity() const noexcept {
			return _capacity;
		}

		constexpr void shrink_to_fit() noexcept {
			if (_size < _capacity)
				_reallocation(_size);
		};

		constexpr void clear() noexcept {
			for (size_t i = 0; i < _size; i++)
				_data[i].~T();
			_size = 0;
		}

		iterator insert(const_iterator pos, const T& value) {
			return emplace(pos, value);
		}

		iterator insert(const_iterator pos, T&& value) {
			return emplace(pos, std::move(value));
		}

		iterator insert(const_iterator pos, size_type count, const T& value) {
			if (pos < cbegin() || pos > cend())
				throw std::out_of_range("vector insert position out of range");

			const difference_type offset = pos - cbegin();
			if (count == 0) return begin() + offset;

			if (count > max_size() - _size)
				throw std::length_error("vector insert maximum size exceeded");

			if (_size + count > _capacity)
				_reallocation(std::max(_size + count, _capacity * 2));

			iterator it = begin() + offset;
			const size_type elems_after = end() - it;

			for (size_type i = 0; i < elems_after; i++) {
				new (&_data[_size + count - 1 - i]) T(std::move(_data[_size - 1 - i]));
				_data[_size - 1 - i].~T();
			}

			for (size_type i = 0; i < count; ++i) {
				new (&_data[offset + i]) T(value);
			}

			_size += count;
			return it;
		}

		template<typename InputIt>
		iterator insert(const_iterator pos, InputIt first, InputIt last) {
			if (pos < begin() || pos > end())
				throw std::out_of_range("vector insert position is out of range");

			difference_type offset = pos - cbegin();
			size_type count = std::distance(first, last);
			if (count == 0) return begin() + offset;

			if (count > max_size() - _size)
				throw std::length_error("vector insert maximum size exceeded");

			if (_size + count > _capacity) {
				_reallocation(std::max(_size + count, _capacity * 2));
			}

			iterator it = begin() + offset;
			const size_type elems_after = end() - it;

			for (size_type i = 0; i < elems_after; i++) {
				new (&_data[_size + count - 1 - i]) T(std::move(_data[_size - 1 - i]));
				_data[_size - 1 - i].~T();
			}

			try {
				for (size_type i = 0; i < count; ++i, ++first) {
					new (&_data[offset + i]) T(*first);
				}
			} catch (...) {
				for (size_type i = 0; i < count; ++i) {
					if (offset + i < _size + count) {
						_data[offset + i].~T();
					}
				}
				throw;
			}

			_size += count;
			return it;
		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
			return insert(pos, ilist.begin(), ilist.end());
		}

		template<typename... Args>
		constexpr iterator emplace(const_iterator pos, Args&&... args) {
			if (pos < cbegin() || pos > cend())
				throw std::out_of_range("vector emplace position out of range");

			const difference_type offset = pos - cbegin();

			if (_size == _capacity)
				_reallocation(_capacity ? _capacity * 2 : 1);

			iterator it = begin() + offset;
			if (it != end()) {
				new (&_data[_size]) T(std::move(_data[_size - 1]));

				std::move_backward(it, end() - 1, end());

				it->~T();
			}

			new (&*it) T(std::forward<Args>(args)...);

			_size++;
			return it;
		}

		iterator erase(const_iterator pos) {
			return erase(pos, pos + 1);
		}

		iterator erase(const_iterator first, const_iterator last) {
			if (first < cbegin() || last > cend() || first > last) {
				throw std::out_of_range("vector::erase - invalid range");
			}

			if (first == last) return begin() + (first - cbegin());

			const difference_type offset = first - cbegin();
			const size_type count = last - first;
			iterator it = begin() + offset;

			for (size_type i = 0; i < count; ++i) {
				_data[offset + i].~T();
			}

			for (size_type i = 0; i < _size - offset - count; ++i) {
				_data[offset + i] = std::move(_data[offset + count + i]);
			}

			for (size_type i = _size - count; i < _size; ++i) {
				_data[i].~T();
			}

			_size -= count;
			return it;
		}

		constexpr void push_back(const T& value) {
			if (_size >= _capacity)
				_reallocation(_capacity ? _capacity * 2 : 1);

			new (&_data[_size]) T(value);
			++_size;
		}

		constexpr void push_back(T&& value) {
			if (_size >= _capacity)
				_reallocation(_capacity ? _capacity * 2 : 1);

			new (&_data[_size]) T(std::move(value));
			++_size;
		}

		template<typename... Args>
		constexpr reference emplace_back(Args&&... args) {
			if (_size >= _capacity)
				_reallocation(_capacity ? _capacity * 2 : 1);

			new (&_data[_size]) T(std::forward<Args>(args)...);
			return _data[_size++];
		}

		constexpr void pop_back() noexcept {
			--_size;
			_data[_size].~T();
		}

		constexpr void resize(size_type newSize) {
			resize(newSize, T());
		}

		constexpr void resize(size_type newSize, const T& value) {
			if (_size < newSize) {
				reserve(newSize);

				for (size_t i = _size; i < newSize; i++)
					new (&_data[i]) T(value);
			} else if (newSize < _size) {
				for (size_t i = newSize; i < _size; i++)
					_data[i].~T();
			}
			_size = newSize;
		}

		constexpr void swap(vector& other) noexcept {
			std::swap(_data, other._data);
			std::swap(_size, other._size);
			std::swap(_capacity, other._capacity);
		}
	private:
		constexpr void _reallocation(const size_t newCapacity) noexcept {
			T* newData = (T*)::operator new(newCapacity * sizeof(T));

			if (newCapacity < _size)
				_size = newCapacity;

			for (size_t i = 0; i < _size; i++)
				new (&newData[i]) T(std::move(_data[i]));

			for (size_t i = 0; i < _size; i++)
				_data[i].~T();

			::operator delete(_data);
			_data = newData;
			_capacity = newCapacity;
		}
	private:
		pointer _data = nullptr;
		size_type _size = 0;
		size_type _capacity = 0;
	};
}

#endif // LZT_VECTOR_H
