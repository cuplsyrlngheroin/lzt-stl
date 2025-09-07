
#ifndef LZT_LIST_H
#define LZT_LIST_H

#include <memory>
#include <utility>
#include <iterator>
#include <stdexcept>

namespace lzt {
	namespace detail {
		struct base_node {
			base_node* next_;
			base_node* prev_;
		};

		template <typename T>
		struct list_node : public base_node {
			T elem_;

			list_node(T&& value) : elem_(std::forward<T>(value)) {}
		};
	}

	template <class list>
	class list_const_iterator {
	public:
		using iterator_category = std::bidirectional_iterator_tag;

		using value_type = typename list::value_type;
		using difference_type = typename list::difference_type;
		using pointer = typename list::const_pointer;
		using reference = const value_type&;
	public:
		list_const_iterator() noexcept : node_(nullptr) {}

		explicit list_const_iterator(detail::base_node* node) noexcept : node_(node) {}

		reference operator*() const noexcept {
			return static_cast<detail::list_node<value_type>*>(node_)->elem_;
		}

		pointer operator->() const noexcept {
			return &static_cast<detail::list_node<value_type>*>(node_)->elem_;
		}

		list_const_iterator& operator++() noexcept {
			node_ = node_->next_;
			return *this;
		}

		list_const_iterator operator++(int) noexcept {
			list_const_iterator temp = *this;
			node_ = node_->next_;
			return temp;
		}

		list_const_iterator& operator--() noexcept {
			node_ = node_->prev_;
			return *this;
		}

		list_const_iterator operator--(int) noexcept {
			list_const_iterator temp = *this;
			node_ = node_->prev_;
			return temp;
		}

		bool operator==(const list_const_iterator& other) const noexcept {
			return node_ == other.node_;
		}

		bool operator!=(const list_const_iterator& other) const noexcept {
			return !(*this == other);
		}
	private:
		detail::base_node* node_;
	};

	template <class list>
	class list_iterator : public list_const_iterator<list> {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = typename list::value_type;
		using difference_type = typename list::difference_type;
		using pointer = typename list::pointer;
		using reference = value_type&;

		using my_base = list_const_iterator<list>;
	public:
		using my_base::my_base;

		reference operator*() const noexcept {
			return const_cast<reference>(my_base::operator*());
		}

		pointer operator->() const noexcept {
			return const_cast<pointer>(my_base::operator->());
		}

		list_iterator& operator++() noexcept {
			my_base::operator++();
			return *this;
		}

		list_iterator operator++(int) noexcept {
			list_iterator temp = *this;
			my_base::operator++();
			return temp;
		}

		list_iterator& operator--() noexcept {
			my_base::operator--();
			return *this;
		}

		list_iterator operator--(int) noexcept {
			list_iterator temp = *this;
			my_base::operator--();
			return temp;
		}
	};

	template <typename T>
	class list {
	public:
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using iterator = list_iterator<list>;
		using const_iterator = list_const_iterator<list>;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	public:
		list() : head_(), tail_(), size_(0) {
			head_.next_ = &tail_;
			tail_.prev_ = &head_;
		}

		list(const list& other) : list() {
			detail::base_node* current = other.head_.next_;
			while (current != &other.tail_) {
				detail::list_node<T>* node = 
					static_cast<detail::list_node<T>*>(current);
				push_back(node->elem_);
				current = current->next_;
			}
		}

		list(list&& other) noexcept : head_(), tail_(), size_() {
			if (!other.empty()) {
				head_.next_ = other.head_.next_;
				tail_.prev_ = other.tail_.prev_;
				size_ = other.size_;

				head_.next_->prev_ = &head_;
				tail_.prev_->next_ = &tail_;

				other.head_.next_ = &other.tail_;
				other.tail_.prev_ = &other.head_;
				other.size_ = 0;
			} else {
				head_.next_ = &tail_;
				tail_.prev_ = &head_;
			}
		}

		~list() {
			clear();
		}

		list& operator=(const list& other) {
			if (this != &other) {
				clear();
				detail::base_node* current = other.head_.next_;
				while (current != &other.tail_) {
					detail::list_node<T>* node =
						static_cast<detail::list_node<T>*>(current);
					push_back(node->elem_);
					current = current->next_;
				}
			}
			return *this;
		}

		list& operator=(list&& other) noexcept {
			if (this != &other) {
				clear();

				head_.next_ = other.head_.next_;
				tail_.prev_ = other.tail_.prev_;
				size_ = other.size_;

				if (!empty()) {
					head_.next_->prev_ = &head_;
					tail_.prev_->next_ = &tail_;
				} else {
					head_.next_ = &tail_;
					tail_.prev_ = &head_;
				}

				other.head_.next_ = &other.tail_;
				other.tail_.prev_ = &other.head_;
				other.size_ = 0;
			}
			return *this;
		}

		reference front() noexcept {
			return static_cast<detail::list_node<T>*>(head_.next_)->elem_;
		}

		const_reference front() const noexcept {
			return static_cast<detail::list_node<T>*>(head_.next_)->elem_;
		}

		reference back() noexcept {
			return static_cast<detail::list_node<T>*>(tail_.prev_)->elem_;
		}

		const_reference back() const noexcept {
			return static_cast<detail::list_node<T>*>(tail_.prev_)->elem_;
		}

		iterator begin() noexcept {
			return iterator(head_.next_);
		}

		const_iterator begin() const noexcept {
			return const_iterator(head_.next_);
		}

		iterator end() noexcept {
			return iterator(&tail_);
		}

		const_iterator end() const noexcept {
			return const_iterator(&tail_);
		}

		reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		const_iterator cbegin() const noexcept {
			return begin();
		}

		const_iterator cend() const noexcept {
			return end();
		}

		const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		const_reverse_iterator crend() const noexcept {
			return rend();
		}

		constexpr bool empty() const noexcept {
			return size_ == 0;
		}

		constexpr size_type size() const noexcept {
			return size_;
		}

		void clear() {
			while (size_ > 0)
				pop_front();
		}

		template <typename... Args>
		iterator emplace(const_iterator pos, Args&&... args) {
			detail::base_node* current;

			if (pos == end()) {
				current = &tail_;
			} else {
				current = head_.next_;
				const_iterator it = begin();

				while (it != pos) {
					current = current->next_;
					++it;
				}
			}

			detail::list_node<T>* new_node =
				(detail::list_node<T>*)::operator new(sizeof(detail::list_node<T>));

			try {
				new (&new_node->elem_) T(std::forward<Args>(args)...);
			} catch (...) {
				::operator delete(new_node);
				throw;
			}

			new_node->next_ = current;
			new_node->prev_ = current->prev_;

			current->prev_->next_ = new_node;
			current->prev_ = new_node;

			++size_;
			return iterator(new_node);
		}

		void push_back(const T& value) {
			detail::list_node<T>* new_node =
				(detail::list_node<T>*)::operator new(sizeof(detail::list_node<T>));

			try {
				new (&new_node->elem_) T(value);
			} catch (...) {
				::operator delete(new_node);
				throw;
			}

			new_node->next_ = &tail_;
			new_node->prev_ = tail_.prev_;

			tail_.prev_->next_ = new_node;
			tail_.prev_ = new_node;

			++size_;
		}

		void push_back(T&& value) {
			emplace(end(), std::move(value));
		}

		template <typename... Args>
		reference emplace_back(Args&&... args) {
			return *emplace(end(), std::forward<Args>(args)...);
		}

		void pop_back() {
			detail::list_node<T>* last_node =
				static_cast<detail::list_node<T>*>(tail_.prev_);
			
			last_node->prev_->next_ = &tail_;
			tail_.prev_ = last_node->prev_;

			last_node->elem_.~T();
			::operator delete(last_node);

			--size_;
		}

		void push_front(const T& value) {
			detail::list_node<T>* new_node =
				(detail::list_node<T>*)::operator new(sizeof(detail::list_node<T>));

			try {
				new (&new_node->elem_) T(value);
			} catch (...) {
				::operator delete(new_node);
				throw;
			}

			new_node->next_ = head_.next_;
			new_node->prev_ = &head_;

			head_.next_->prev_ = new_node;
			head_.next_ = new_node;

			++size_;
		}

		void push_front(T&& value) {
			emplace(begin(), std::move(value));
		}

		template <typename... Args>
		reference emplace_front(Args&&... args) {
			return *emplace(begin(), std::forward<Args>(args)...);
		}

		void pop_front() {
			detail::list_node<T>* first_node =
				static_cast<detail::list_node<T>*>(head_.next_);

			first_node->next_->prev_ = &head_;
			head_.next_ = first_node->next_;

			first_node->elem_.~T();
			::operator delete(first_node);

			--size_;
		}

		void swap(list& other) noexcept {
			std::swap(head_.next_, other.head_.next_);
			std::swap(head_.prev_, other.head_.prev_);
			std::swap(tail_.next_, other.tail_.next_);
			std::swap(tail_.prev_, other.tail_.prev_);
			std::swap(size_, other.size_);

			if (head_.next_) head_.next_->prev_ = &head_;
			if (tail_.prev_) tail_.prev_->next_ = &tail_;
			if (other.head_.next_) other.head_.next_->prev_ = &other.head_;
			if (other.tail_.prev_) other.tail_.prev_->next_ = &other.tail_;
		}
	private:
		detail::base_node head_;
		detail::base_node tail_;
		size_t size_ = 0;
	};
}

#endif // LZT_LIST_H