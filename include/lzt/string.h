
#ifndef LZT_STRING_H
#define LZT_STRING_H

#include <iostream>
#include <cctype>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

namespace lzt {
    template<typename basic_string>
    class string_const_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename basic_string::value_type;
        using difference_type = typename basic_string::difference_type;
        using pointer = typename basic_string::const_pointer;
        using reference = typename basic_string::const_reference;
    public:
        constexpr string_const_iterator() noexcept = default;

        constexpr explicit string_const_iterator(pointer ptr) : _pointer(ptr) {}

        constexpr reference operator*() const noexcept {
            return *_pointer;
        }

        constexpr pointer operator->() const noexcept {
            return _pointer;
        }

        constexpr string_const_iterator& operator++() noexcept {
            ++_pointer;
            return *this;
        }

        constexpr string_const_iterator operator++(int) noexcept {
            string_const_iterator temp = *this;
            ++(*this);
            return temp;
        }

        constexpr string_const_iterator& operator--() noexcept {
            --_pointer;
            return *this;
        }

        constexpr string_const_iterator operator--(int) noexcept {
            string_const_iterator temp = *this;
            --(*this);
            return temp;
        }

        constexpr string_const_iterator& operator+=(const difference_type offset) noexcept {
            _pointer += offset;
            return *this;
        }

        constexpr string_const_iterator operator+(const difference_type offset) const noexcept {
            return string_const_iterator(_pointer + offset);
        }

        friend constexpr string_const_iterator operator+(
            const difference_type offset, string_const_iterator next) noexcept {
            next += offset;
            return next;
        }

        constexpr string_const_iterator& operator-=(const difference_type offset) noexcept {
            _pointer -= offset;
            return *this;
        }

        constexpr string_const_iterator operator-(const difference_type offset) const noexcept {
            return string_const_iterator(_pointer - offset);
        }

        constexpr difference_type operator-(const string_const_iterator& other) const noexcept {
            return static_cast<difference_type>(_pointer - other._pointer);
        }

        constexpr reference operator[](const difference_type offset) const noexcept {
            return *(*this + offset);
        }

        constexpr bool operator==(const string_const_iterator& other) const noexcept {
            return _pointer == other._pointer;
        }

        constexpr bool operator!=(const string_const_iterator& other) const noexcept {
            return !(*this == other);
        }

        constexpr bool operator<(const string_const_iterator& other) const noexcept {
            return _pointer < other._pointer;
        }

        constexpr bool operator>(const string_const_iterator& other) const noexcept {
            return other < *this;
        }

        constexpr bool operator<=(const string_const_iterator& other) const noexcept {
            return !(other < *this);
        }

        constexpr bool operator>=(const string_const_iterator& other) const noexcept {
            return !(*this < other);
        }
    protected:
        pointer _pointer;
    };

    template<typename basic_string>
    class string_iterator : public string_const_iterator<basic_string> {
    public:
        using myBase = string_const_iterator<basic_string>;

        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename basic_string::value_type;
        using difference_type = typename basic_string::difference_type;
        using pointer = typename basic_string::pointer;
        using reference = typename basic_string::reference;
    public:
        using myBase::myBase;

        constexpr reference operator*() const noexcept {
            return const_cast<reference>(myBase::operator*());
        }

        constexpr pointer operator->() const noexcept {
            return const_cast<pointer>(myBase::operator->());
        }

        constexpr string_iterator& operator++() noexcept {
            myBase::operator++();
            return *this;
        }

        constexpr string_iterator operator++(int) noexcept {
            string_iterator temp = *this;
            myBase::operator++();
            return temp;
        }

        constexpr string_iterator& operator--() noexcept {
            myBase::operator--();
            return *this;
        }

        constexpr string_iterator operator--(int) noexcept {
            string_iterator temp = *this;
            myBase::operator--();
            return temp;
        }

        constexpr string_iterator& operator+=(const difference_type offset) noexcept {
            myBase::operator+=(offset);
            return *this;
        }

        constexpr string_iterator operator+(const difference_type offset) const noexcept {
            return string_iterator(this->_pointer + offset);
        }

        friend constexpr string_iterator operator+(
            const difference_type offset, string_iterator next) noexcept {
            next += offset;
            return next;
        }

        constexpr string_iterator& operator-=(const difference_type offset) noexcept {
            myBase::operator-=(offset);
            return *this;
        }

        using myBase::operator-;

        constexpr string_iterator operator-(const difference_type offset) const noexcept {
            return string_iterator(this->_pointer - offset);
        }

        constexpr reference operator[](const difference_type offset) const noexcept {
            return const_cast<reference>(myBase::operator[](offset));
        }
    };

    template<typename CharT>
    class basic_string {
    public:
        using value_type = CharT;
        using size_type = size_t;
        using pointer = CharT*;
        using const_pointer = const CharT*;
        using reference = CharT&;
        using const_reference = const CharT&;

        using const_iterator = string_const_iterator<basic_string>;
        using iterator = string_iterator<basic_string>;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        static constexpr size_type npos = static_cast<size_type>(-1);

        basic_string() noexcept = default;

        basic_string(size_type count, CharT ch) {
            if (count == 0) {
                _size = 0;
                _capacity = 1;
                _allocate(_capacity);
                _elems[0] = CharT();
                return;
            }

            _size = count;
            _capacity = count + 1;
            _allocate(_capacity);
            for (size_type i = 0; i < count; ++i)
                new (&_elems[i]) CharT(ch);
            _elems[_size] = CharT();
        }

        basic_string(const CharT* s, size_type count) {
            if (!s || count == 0) {
                _size = 0;
                _capacity = 1;
                _allocate(_capacity);
                _elems[0] = CharT();
                return;
            }

            _size = count;
            _capacity = count + 1;
            _allocate(_capacity);
            for (size_type i = 0; i < count; ++i)
                new (&_elems[i]) CharT(s[i]);
            _elems[_size] = CharT();
        }

        basic_string(const CharT* s) {
            if (!s) {
                _size = 0;
                _capacity = 1;
                _allocate(_capacity);
                _elems[0] = CharT();
                return;
            }

            _size = std::char_traits<CharT>::length(s);
            _capacity = _size + 1;
            _allocate(_capacity);
            for (size_t i = 0; i < _size; ++i)
                new (&_elems[i]) CharT(s[i]);
            _elems[_size] = CharT();
        }

        basic_string(std::nullptr_t) = delete;

        basic_string(const basic_string& other)
            : _size(other._size), _capacity(other._capacity) {

            _allocate(other._capacity);
            for (size_t i = 0; i < _size; ++i)
                new (&_elems[i]) CharT(other._elems[i]);
            _elems[_size] = CharT();
        }

        basic_string& operator=(const basic_string& other) {
            if (this != &other) {
                basic_string temp(other);
                swap(temp);
            }
            return *this;
        }

        basic_string(basic_string&& other) noexcept
            : _elems(other._elems), _size(other._size), _capacity(other._capacity) {

            other._elems = nullptr;
            other._size = 0;
            other._capacity = 0;
        }

        basic_string& operator=(basic_string&& other) noexcept {
            if (this != &other) {
                swap(other);
            }
            return *this;
        }

        basic_string(std::initializer_list<CharT> init)
            : _size(init.size()), _capacity(init.size() + 1) {

            _allocate(_capacity);
            size_t i = 0;
            for (auto& ch : init) {
                new (&_elems[i++]) CharT(ch);
            }
            _elems[_size] = CharT();
        }

        ~basic_string() {
            _deallocate();
        }

        constexpr reference at(const size_type index) {
            if (size() <= index)
                throw std::out_of_range("index is out of range");
            return _elems[index];
        }

        constexpr const_reference at(const size_type index) const {
            if (size() <= index)
                throw std::out_of_range("index is out of range");
            return _elems[index];
        }

        constexpr reference operator[](const size_type index) noexcept {
            return _elems[index];
        }

        constexpr const_reference operator[](const size_type index) const noexcept {
            return _elems[index];
        }

        constexpr reference front() noexcept {
            return _elems[0];
        }

        constexpr const_reference front() const noexcept {
            return _elems[0];
        }

        constexpr reference back() noexcept {
            return _elems[_size - 1];
        }

        constexpr const_reference back() const noexcept {
            return _elems[_size - 1];
        }

        constexpr pointer data() noexcept {
            return _elems;
        }

        constexpr const_pointer data() const noexcept {
            return _elems;
        }

        constexpr pointer c_str() noexcept {
            return _elems;
        }

        constexpr const_pointer c_str() const noexcept {
            return _elems;
        }

        constexpr iterator begin() noexcept {
            return iterator(_elems);
        }

        constexpr const_iterator begin() const noexcept {
            return const_iterator(_elems);
        }

        constexpr iterator end() noexcept {
            return iterator(_elems + _size);
        }

        constexpr const_iterator end() const noexcept {
            return const_iterator(_elems + _size);
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

        constexpr size_type length() const noexcept {
            return _size;
        }

        constexpr size_type max_size() const noexcept {
            return std::numeric_limits<size_t>::max();
        }

        void reserve(size_type newCapacity) {
            if (capacity() - 1 < newCapacity)
                _reallocate(newCapacity + 1);
        }

        constexpr size_type capacity() const noexcept {
            return _capacity;
        }

        void shrink_to_fit() {
            if (size() == 0)
                _reallocate(1);

            if (size() < capacity())
                _reallocate(size() + 1);
        }

        constexpr void clear() {
            for (size_t i = 0; i < size(); ++i)
                _elems[i].~CharT();
            _size = 0;
            _elems[0] = CharT();
        }

        basic_string& insert(const size_type index, const size_type count, CharT ch) {
            if (index > size())
                throw std::out_of_range("insert: index is out of range");

            if(size() + count > capacity())
                reserve(size() + count);

            for (size_type i = size(); i > index; --i) {
                new (&_elems[i + count - 1]) CharT(std::move(_elems[i - 1]));
                _elems[i - 1].~CharT();
            }

            for (size_type i = 0; i < count; ++i)
                new (&_elems[index + i]) CharT(ch);

            _size += count;
            _elems[_size] = CharT();
            return *this;
        }

        basic_string& insert(const size_type index, const CharT* s) {
            size_type len_s = std::char_traits<CharT>::length(s);
            return insert(index, s, len_s);
        }

        basic_string& insert(const size_type index, const CharT* s, const size_type count) {
            if (index > size())
                throw std::out_of_range("insert: index is out of range");

            if (size() + count > capacity())
                reserve(size() + count);

            for (size_type i = size(); i > index; --i) {
                new (&_elems[i + count - 1]) CharT(std::move(_elems[i - 1]));
                _elems[i - 1].~CharT();
            }

            for (size_type i = 0; i < count; ++i)
                new (&_elems[index + i]) CharT(s[i]);

            _size += count;
            _elems[_size] = CharT();
            return *this;
        }

        basic_string& insert(const size_type index, const basic_string& str) {
            return insert(index, str.data(), str.size());
        }

        basic_string& insert(const size_type index, const basic_string& str,
                             const size_type s_index, const size_type count = npos) {
            if (s_index > str.size())
                throw std::out_of_range("insert: s_index is out of range");

            size_type real_count = std::min(count, str.size() - s_index);
            return insert(index, str.data() + s_index, real_count);
        }

        iterator insert(const_iterator pos, CharT ch) {
            size_type index = pos - cbegin();
            insert(index, 1, ch);
            return begin() + index;
        }

        iterator insert(const_iterator pos, const size_type count, CharT ch) {
            size_type index = pos - cbegin();
            insert(index, count, ch);
            return begin() + index;
        }

        template<typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            size_type index = pos - cbegin();
            if (first == last) return begin() + index;

            size_type count = std::distance(first, last);
            
            if (size() + count > capacity())
                reserve(size() + count);

            for (size_type i = size(); i > index; --i) {
                new (&_elems[i + count - 1]) CharT(std::move(_elems[i - 1]));
                _elems[i - 1].~CharT();
            }

            for (size_type i = 0; first != last; ++first, ++i)
                new (&_elems[index + i]) CharT(*first);

            _size += count;
            _elems[_size] = CharT();
            return begin() + index;
        }

        iterator insert(const_iterator pos, std::initializer_list<CharT> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        basic_string& erase(size_type index = 0, size_type count = npos) {
            if (index > size())
                throw std::out_of_range("erase: index is out of range");

            size_type real_count = std::min(count, size() - index);
            if (real_count == 0) return *this;
            
            for (size_type i = index + real_count; i < size(); ++i) {
                new (&_elems[i - real_count]) CharT(std::move(_elems[i]));
                _elems[i].~CharT();
            }

            _size -= real_count;
            _elems[_size] = CharT();
            return *this;
        }

        iterator erase(const_iterator position) {
            if (position < cbegin() || position >= cend())
                throw std::out_of_range("erase: position is out of range");

            size_type index = position - cbegin();
            erase(index, 1);
            return begin() + index;
        }

        iterator erase(const_iterator first, const_iterator last) {
            if (first < cbegin() || last > cend() || first > last)
                throw std::out_of_range("erase: iterator range is out of range");

            size_type index = first - cbegin();
            size_type count = last - first;
            erase(index, count);
            return begin() + index;
        }

        void push_back(const CharT& ch) noexcept {
            if (size() == capacity())
                _reallocate(capacity() * 2 + 1);

            new (&_elems[_size++]) CharT(ch);
            _elems[_size] = CharT();
        }

        void push_back(CharT&& ch) noexcept {
            if (size() == capacity())
                _reallocate(capacity() * 2 + 1);

            new (&_elems[_size++]) CharT(std::move(ch));
            _elems[_size] = CharT();
        }

        void pop_back() noexcept {
            --_size;
            _elems[_size].~CharT();
            _elems[_size] = CharT();
        }

        basic_string& append(size_type count, CharT ch) {
            if (count == 0) return *this;
            
            reserve(size() + count);
            for (size_type i = 0; i < count; ++i)
                new (&_elems[_size++]) CharT(ch);
            _elems[_size] = CharT();
            return *this;
        }

        basic_string& append(const CharT* s, size_type count) {
            if (count == 0) return *this;

            reserve(size() + count);
            for (size_type i = 0; i < count; ++i)
                new (&_elems[_size++]) CharT(s[i]);
            _elems[_size] = CharT();
            return *this;
        }

        basic_string& append(const CharT* s) {
            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0) return *this;

            reserve(_size + len_s);
            for (size_type i = 0; i < len_s; ++i)
                new (&_elems[_size++]) CharT(s[i]);
            _elems[_size] = CharT();
            return *this;
        }

        basic_string& append(const basic_string& str) {
            return append(str.c_str(), str.size());
        }

        basic_string& append(const basic_string& str, size_type pos, size_type count = npos) {
            if (pos > str.size())
                throw std::out_of_range("append: pos out of range");

            size_type actual_count = std::min(count, str.size() - pos);
            return append(str.c_str() + pos, actual_count);
        }

        basic_string& append(std::initializer_list<CharT> ilist) {
            return append(ilist.begin(), ilist.size());
        }

        basic_string& operator+=(const basic_string& str) {
            return append(str.c_str(), str.size());
        }

        basic_string& operator+=(CharT ch) {
            return append(1, ch);
        }

        basic_string& operator+=(const CharT* s) {
            return append(s);
        }

        basic_string& operator+=(std::initializer_list<CharT> ilist) {
            return append(ilist.begin(), ilist.size());
        }

        basic_string& replace(const size_type pos, const size_type count,
                              const basic_string& str) {
            erase(pos, count);
            insert(pos, str);
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last,
                              const basic_string& str) {
            size_type index = first - cbegin();
            size_type count = last - first;

            erase(index, count);
            insert(index, str);
            return *this;
        }

        basic_string& replace(const size_type pos1, const size_type count1,
                              const basic_string& str,
                              const size_type pos2, const size_type count2 = npos) {
            if (pos2 > str.size())
                throw std::out_of_range("replce: pos2 is out of range");

            size_type real_count2 = std::min(count2, str.size() - pos2);
            erase(pos1, count1);
            insert(pos1, str.data() + pos2, real_count2);
            return *this;
        }

        basic_string& replace(const size_type pos, const size_type count1,
                              const CharT* s, size_type count2) {
            erase(pos, count1);
            insert(pos, s, count2);
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last,
                              const CharT* s, const size_type count2) {
            size_type pos = first - cbegin();
            size_type count1 = last - first;
            erase(pos, count1);
            insert(pos, s, count2);
            return* this;
        }

        basic_string& replace(const size_type pos, const size_type count,
                              const CharT* s) {
            erase(pos, count);
            insert(pos, s, count);
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last,
                              const CharT* s) {
            size_type pos = first - cbegin();
            size_type count = last - first;
            size_type len_s = std::char_traits<CharT>::length(s);
            erase(pos, count);
            insert(pos, s, len_s);
            return *this;
        }

        basic_string& replace(const size_type pos, const size_type count1,
                              const size_type count2, CharT ch) {
            erase(pos, count1);
            insert(pos, count2, ch);
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last,
                              size_type count2, CharT ch) {
            size_type pos = first - cbegin();
            size_type count1 = last - first;
            erase(pos, count1);
            insert(pos, count2, ch);
            return *this;
        }

        template< class InputIt >
        basic_string& replace(const_iterator first, const_iterator last,
                              InputIt first2, InputIt last2) {
            size_type pos = first - cbegin();
            size_type count1 = last - first;
            erase(pos, count1);
            insert(pos, first2, last2);
            return *this;
        }

        basic_string& replace(const_iterator first, const_iterator last,
                              std::initializer_list<CharT> ilist) {
            size_type pos = first - cbegin();
            size_type count = last - first;
            erase(pos, count);
            insert(pos, ilist);
            return *this;
        }

        size_type copy(CharT* dest, size_type count, size_type pos = 0) const {
            if (pos > size())
                throw std::out_of_range("copy: pos is out of range");

            size_type real_count = std::min(count, size() - pos);

            for (size_type i = 0; i < real_count; ++i) 
                dest[i] = _elems[pos + i];

            return real_count;
        }

        void resize(size_type newSize) {
            resize(newSize, CharT());
        }

        void resize(size_type newSize, const CharT& ch) {
            if (newSize > max_size())
                throw std::length_error("basic_string::resize");

            if (_size < newSize) {
                reserve(newSize + 1);

                for (size_t i = _size; i < newSize; ++i)
                    new (&_elems[i]) CharT(ch);
            }
            else if (_size > newSize) {
                for (size_t i = newSize; i < _size; ++i)
                    _elems[i].~CharT();
            }
            _size = newSize;
            _elems[_size] = CharT();
        }

        void swap(basic_string& other) noexcept {
            std::swap(_elems, other._elems);
            std::swap(_size, other._size);
            std::swap(_capacity, other._capacity);
        }

        size_type find(const basic_string& str, size_type pos = 0) const {
            if (pos > size())
                return npos;

            if (str.size() == 0)
                return pos;

            return find(str.c_str(), pos, str.size());
        }

        size_type find(const CharT* s, size_type pos, size_type count) const {
            if (pos > size())
                return npos;
            size_type len_s = std::char_traits<CharT>::length(s);
            size_type search_len = (count < len_s) ? count : len_s;

            if (search_len == 0)
                return pos;

            if (pos + search_len > size())
                return npos;

            for (size_type i = pos; i <= size() - search_len; ++i) {
                if (std::char_traits<CharT>::compare(_elems + i, s, search_len) == 0)
                    return i;
            }
            return npos;
        }

        size_type find(const CharT* s, size_type pos = 0) const {
            if (pos > size())
                return npos;

            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return pos;

            for (size_type i = pos; i < size(); ++i) {
                if (std::char_traits<CharT>::compare(_elems + i, s, len_s) == 0)
                    return i;
            }
            return npos;
        }

        size_type find(CharT ch, size_type pos = 0) const {
            if (pos > size())
                return npos;

            for (size_type i = pos; i < size(); ++i) {
                if (_elems[i] == ch)
                    return i;
            }
            return npos;
        }

        size_type rfind(const basic_string& str, size_type pos = npos) const {
            if (str.size() == 0)
                return std::min(pos, size());

            return rfind(str.c_str(), pos, str.size());
        }

        size_type rfind(const CharT* s, size_type pos, size_type count) const {
            size_type len_s = std::char_traits<CharT>::length(s);
            size_type search_len = (count < len_s) ? count : len_s;

            if (search_len == 0)
                return std::min(pos, size());

            if (search_len > size())
                return npos;

            size_type start = (pos == npos || pos > size() - search_len) ? size() - search_len : pos;
            for (size_type i = start; i != npos; --i) {
                if (std::char_traits<CharT>::compare(_elems + i, s, search_len) == 0) {
                    return i;
                }
                if (i == 0) break;
            }
            return npos;
        }

        size_type rfind(const CharT* s, size_type pos = npos) const {
            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return std::min(pos, size());

            if (len_s > size())
                return npos;
            
            size_type start = (pos == npos || pos > size() - len_s) ? size() - len_s : pos;
            for (size_type i = start; i != npos; --i) {
                if (std::char_traits<CharT>::compare(_elems + i, s, len_s) == 0)
                    return i;

                if (i == 0) break;
            }
            return npos;
        }

        size_type rfind(CharT ch, size_type pos = npos) const {
            if (size() == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                if (_elems[i] == ch)
                    return i;
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_first_of(const basic_string& str, size_type pos = 0) const {
            return find_first_of(str.c_str(), pos, str.size());
        }

        size_type find_first_of(const CharT* s, size_type pos, size_type count) const {
            if (count == 0 || pos > size())
                return npos;

            for (size_type i = pos; i < size(); ++i) {
                for (size_type j = 0; j < count; ++j) {
                    if (_elems[i] == s[j])
                        return i;
                }
            }
            return npos;
        }

        size_type find_first_of(const CharT* s, size_type pos = 0) const {
            if (pos > size())
                return npos;

            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return npos;

            for (size_type i = pos; i < size(); ++i) {
                for (size_type j = 0; j < len_s; ++j) {
                    if (_elems[i] == s[j])
                        return i;
                }
            }
            return npos;
        }

        size_type find_first_of(CharT ch, size_type pos = 0) const {
            if (pos > size())
                return npos;

            for (size_type i = pos; i < size(); ++i) {
                if (_elems[i] == ch)
                    return i;
            }
            return npos;
        }

        size_type find_first_not_of(const basic_string& str, size_type pos = 0) const {
            return find_first_not_of(str.c_str(), pos, str.size());
        }

        size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const {
            if (pos > size())
                return npos;
            
            for (size_type i = pos; i < size(); ++i) {
                bool found = false;
                for (size_type j = 0; j < count; ++j) {
                    if (_elems[i] == s[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) return i;
            }
            return npos;
        }

        size_type find_first_not_of(const CharT* s, size_type pos = 0) const {
            if (pos > size())
                return npos;

            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return pos;

            for (size_type i = pos; i < size(); ++i) {
                bool found = false;
                for (size_type j = 0; j < len_s; ++j) {
                    if (_elems[i] == s[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) return i;
            }
            return npos;
        }

        size_type find_first_not_of(CharT ch, size_type pos = 0) const {
            if (pos > size())
                return npos;

            for (size_type i = pos; i < size(); ++i) {
                if (_elems[i] != ch)
                    return i;
            }
            return npos;
        }

        size_type find_last_of(const basic_string& str, size_type pos = npos) const {
            return find_last_of(str.c_str(), pos, str.size());
        }

        size_type find_last_of(const CharT* s, size_type pos, size_type count) const {
            if (size() == 0)
                return npos;

            if (count == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                for (size_type j = 0; j < count; ++j) {
                    if (_elems[i] == s[j])
                        return i;
                }
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_last_of(const CharT* s, size_type pos = npos) const {
            if (size() == 0)
                return npos;

            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                for (size_type j = 0; j < len_s; ++j) {
                    if (_elems[i] == s[j])
                        return i;
                }
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_last_of(CharT ch, size_type pos = npos) const {
            if (size() == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                if (_elems[i] == ch)
                    return i;
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_last_not_of(const basic_string& str, size_type pos = npos) const {
            return find_last_not_of(str.c_str(), pos, str.size());
        }

        size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const {
            if (size() == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                bool found = false;
                for (size_type j = 0; j < count; ++j) {
                    if (_elems[i] == s[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) return i;
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_last_not_of(const CharT* s, size_type pos = npos) const {
            if (size() == 0)
                return npos;

            size_type len_s = std::char_traits<CharT>::length(s);
            if (len_s == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                bool found = false;
                for (size_type j = 0; j < len_s; ++j) {
                    if (_elems[i] == s[j]) {
                        found = true;
                        break;
                    }
                }
                if (!found) return i;
                if (i == 0) break;
            }
            return npos;
        }

        size_type find_last_not_of(CharT ch, size_type pos = npos) const {
            if (size() == 0)
                return npos;

            size_type start = (pos >= size()) ? size() - 1 : pos;
            for (size_type i = start; i != npos; --i) {
                if (_elems[i] != ch)
                    return i;
                if (i == 0) break;
            }
            return npos;
        }

        int compare(const basic_string& str) const noexcept {
            size_type min_len = std::min(size(), str.size());
            int result = std::char_traits<CharT>::compare(data(), str.data(), min_len);
            if (result != 0) return result;
            if (size() < str.size()) return -1;
            if (size() > str.size()) return 1;
            return 0;
        }

        int compare(size_type pos1, size_type count1,
                    const basic_string& str) const {
            return compare(pos1, count1, str.c_str());
        }

        int compare(size_type pos1, size_type count1,
                    const basic_string& str,
                    size_type pos2, size_type count2 = npos) const {
            if (pos1 > size())
                throw std::out_of_range("compare: pos1 is out of range");
            if (pos2 > str.size())
                throw std::out_of_range("compare: pos2 is out of range");

            size_type real_count2 = std::min(count2, str.size() - pos2);

            return compare(pos1, count1, str.data() + pos2, real_count2);
        }

        int compare(const CharT* s) const {
            size_type len_s = std::char_traits<CharT>::length(s);
            size_type min_len = std::min(size(), len_s);
            int result = std::char_traits<CharT>::compare(data(), s, min_len);
            if (result != 0) return result;
            if (size() < len_s) return -1;
            if (size() > len_s) return 1;
            return 0;
        }

        int compare(size_type pos1, size_type count1,
                    const CharT* s) const {
            if (pos1 > size())
                throw std::out_of_range("compare: pos1 out of range");

            size_type real_count1 = std::min(count1, size() - pos1);
            size_type len_s = std::char_traits<CharT>::length(s);
            size_type min_len = std::min(real_count1, len_s);

            int result = std::char_traits<CharT>::compare(data() + pos1, s, min_len);
            if (result != 0) return result;
            if (real_count1 < len_s) return -1;
            if (real_count1 > len_s) return 1;
            return 0;
        }

        int compare(size_type pos1, size_type count1,
                    const CharT* s, size_type count2) const {
            if (pos1 > size())
                throw std::out_of_range("compare: pos1 out of range");

            size_type real_count1 = std::min(count1, size() - pos1);
            size_type min_len = std::min(real_count1, count2);

            int result = std::char_traits<CharT>::compare(data() + pos1, s, min_len);
            if (result != 0) return result;
            if (real_count1 < count2) return -1;
            if (real_count1 > count2) return 1;
            return 0;
        }

        basic_string substr(size_type pos = 0, size_type count = npos) const {
            if (pos > size())
                throw std::out_of_range("substr: pos is out of range");

            size_type real_count = std::min(count, size() - pos);
            return basic_string(data() + pos, real_count);
        }
    private:
        void _allocate(size_t capacity) {
            _elems = (CharT*)::operator new(capacity * sizeof(CharT));
            _capacity = capacity;
        }

        void _reallocate(size_t newCapacity) {
            if (newCapacity == 0)
                newCapacity = 1;

            if (newCapacity < size() + 1)
                newCapacity = size() + 1;

            CharT* newData = (CharT*)::operator new(newCapacity * sizeof(CharT));

            for (size_t i = 0; i < size(); ++i) {
                new (&newData[i]) CharT(std::move(_elems[i]));
                _elems[i].~CharT();
            }

            ::operator delete(_elems);
            _elems = newData;
            _capacity = newCapacity;
            _elems[_size] = CharT();
        }

        void _deallocate() {
            for (size_t i = 0; i < size(); ++i)
                _elems[i].~CharT();

            ::operator delete(_elems);
            _elems = nullptr;
            _size = 0;
            _capacity = 0;
        }
    private:
        CharT* _elems = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;
    };

    template<typename CharT>
    std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const basic_string<CharT>& str) {
        return os << str.c_str();
    }

    template<typename CharT>
    std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, basic_string<CharT>& str) {
        str.clear();
        CharT ch;
        while (is.get(ch) && !std::isspace(ch, is.getloc())) {
            str.push_back(ch);
        }
        return is;
    }

    using string = basic_string<char>;
    using wstring = basic_string<wchar_t>;
    using u16string = basic_string<char16_t>;
    using u32string = basic_string<char32_t>;
}

#endif // LZT_STRING_H