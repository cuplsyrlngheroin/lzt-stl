
#ifndef LZT_LIST_H
#define LZT_LIST_H

#include <memory>
#include <utility>
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
			detail::list_node<T>* new_node =
				(detail::list_node<T>*)::operator new(sizeof(detail::list_node<T>));

			try {
				new (&new_node->elem_) T(std::move(value));
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
			detail::list_node<T>* new_node =
				(detail::list_node<T>*)::operator new(sizeof(detail::list_node<T>));

			try {
				new (&new_node->elem_) T(std::move(value));
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

		void pop_front() {
			detail::list_node<T>* first_node =
				static_cast<detail::list_node<T>*>(head_.next_);

			first_node->next_->prev_ = &head_;
			head_.next_ = first_node->next_;

			first_node->elem_.~T();
			::operator delete(first_node);

			--size_;
		}
	private:
		detail::base_node head_;
		detail::base_node tail_;
		size_t size_ = 0;
	};
}

#endif // LZT_LIST_H