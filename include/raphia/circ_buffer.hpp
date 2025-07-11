#ifndef RAPHIA_CIRC_BUFFER_HPP
#define RAPHIA_CIRC_BUFFER_HPP
#include <stdexcept>
#include <memory>

namespace raphia
{
    /** circ_buffer
     * @brief STL compatible container with circular buffer logic
     */
    template <class T, class Alloc = std::allocator<T>>
    class circ_buffer
    {
    public:
        /** basic_iterator
         * @brief bidirectional iterator
         */
        template <class Container, class ValueType>
        struct basic_iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ValueType;
            using pointer = value_type *;
            using reference = value_type &;
            using container = Container;

            basic_iterator(difference_type offset, container &circ_buffer)
                : offset_(offset), circ_(circ_buffer) {}

            reference operator*() const;
            pointer operator->();
            basic_iterator &operator++();
            basic_iterator operator++(int);
            basic_iterator &operator--();
            basic_iterator operator--(int);
            typename basic_iterator::difference_type operator-(const basic_iterator &it) const;

            friend bool operator==(const basic_iterator &a, const basic_iterator &b) { return a.offset_ == b.offset_; };
            friend bool operator!=(const basic_iterator &a, const basic_iterator &b) { return a.offset_ != b.offset_; };

        private:
            difference_type offset_;
            container &circ_;
        };

        using value_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;
        using size_type = std::size_t;
        using iterator = basic_iterator<circ_buffer<T, Alloc>, T>;
        using const_iterator = basic_iterator<const circ_buffer<T, Alloc>, const T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        /** Constructors **/

        /** circ_buffer
         * @brief default constructor
         */
        circ_buffer(const Alloc &a = Alloc());

        /** circ_buffer
         * @brief constructor
         * @param count initial buffer size
         */
        explicit circ_buffer(size_type size, const Alloc &a = Alloc());

        /** circ_buffer
         * @brief construct container from a range
         */
        template <class Iter>
        circ_buffer(Iter begin, Iter end, const Alloc &a = Alloc());

        /** circ_buffer
         * @brief copy constructor
         */
        circ_buffer(const circ_buffer &);

        /** circ_buffer
         * @brief move constructor
         */
        circ_buffer(circ_buffer &&) noexcept;

        /** Destructor **/

        /** ~circ_buffer
         * @brief deconstructor
         */
        virtual ~circ_buffer();

        /** Copy/Move operators **/

        /** operator=
         * @brief copy operator
         */
        circ_buffer<T, Alloc> &operator=(const circ_buffer &);

        /** operator=
         * @brief move operator
         */
        circ_buffer<T, Alloc> &operator=(circ_buffer &&) noexcept;

        /** Iterators **/

        /** begin
         * @brief retrieves an iterator the first element
         * @return iterator to the first element
         */
        iterator begin() noexcept;

        /** end
         * @brief retrieves an iterator the last element
         * @return iterator to the last element
         */
        iterator end() noexcept;

        /** cbegin
         * @brief retrieves a constant iterator the first element
         * @return constant iterator to the first element
         */
        const_iterator cbegin() const noexcept;

        /** cend
         * @brief retrieves a constant iterator the last element
         * @return constant iterator to the last element
         */
        const_iterator cend() const noexcept;

        /** rbegin
         * @brief retrieves a reverse iterator the first element
         * @return reverse iterator to the first element
         */
        reverse_iterator rbegin() noexcept;

        /** rend
         * @brief retrieves a reverse iterator the last element
         * @return reverse iterator to the last element
         */
        reverse_iterator rend() noexcept;

        /** crbegin
         * @brief retrieves a constant reverse iterator the first element
         * @return constant reverse iterator to the first element
         */
        const_reverse_iterator crbegin() const noexcept;

        /** crend
         * @brief retrieves a constant reverse iterator the last element
         * @return constant reverse iterator to the last element
         */
        const_reverse_iterator crend() const noexcept;

        /** Modifiers **/

        /** push_back
         * @brief add a value to the end of the circular buffer,
         * if the buffer is full, the first element will be overwritten
         * @param a value to be added
         */
        void push_back(value_type &&a);

        /** push_back
         * @brief add a value to the end of the circular buffer,
         * if the buffer is full, the first element will be overwritten
         * @param a value to be added
         */
        void push_back(const value_type &a);

        /** push_front
         * @brief add a value to the front of the circular buffer,
         * if the buffer is full, the last element will be overwritten
         * @param a value to be added
         */
        void push_front(value_type &&a);

        /** push_front
         * @brief push a value to the front of the circular buffer,
         * if the buffer is full, the last element will be overwritten
         * @param a value to be added
         */
        void push_front(const value_type &a);

        /** pop_front
         * @brief remove the first element from the buffer
         */
        void pop_front();

        /** pop_front
         * @brief remove the last element from the buffer
         */
        void pop_back();

        /** emblace_front
         * @brief constructs a new object at the front of the buffer
         * @returns a reference to the newly constructed object
         */
        template <class... Args>
        reference emblace_front(Args &&...args);

        /** emblace_back
         * @brief constructs a new object at the back of the buffer
         * @returns a reference to the newly constructed object
         */
        template <class... Args>
        reference emblace_back(Args &&...args);

        /** clear
         * @brief clear the buffer
         */
        void clear() noexcept;

        /** set_capacity
         * @brief set_capacity the buffer
         */
        void set_capacity(size_type);

        /** Capacity Methods **/

        /** size
         * @brief return the current count of elements in the buffer
         * @return current element count in the buffer
         */
        size_type size() const noexcept;

        /** empty
         * @brief check whether the buffer is empty
         * @return true if buffer is empty
         */
        bool empty() const noexcept;

        /** capacity
         * @brief get the buffer capacity
         * @return buffer capacity
         */
        size_type capacity() const noexcept;

        /** Accessors **/

        /** front
         * @brief access the first element in the buffer
         * @return reference to the first element
         * @throw overflow exception if the buffer is empty
         */
        reference &front();

        /** front
         * @brief access the first element in the buffer
         * @return const reference to the first element
         * @throw overflow exception if the buffer is empty
         */
        const_reference &front() const;

        /** back
         * @brief access the last element in the buffer
         * @return reference to the last element
         * @throw overflow exception if the buffer is empty
         */
        reference &back();

        /** back
         * @brief access the last element in the buffer
         * @return const reference to the last element
         * @throw overflow exception if the buffer is empty
         */
        const_reference &back() const;

        /** operator[]
         * @brief access an element by index
         * @return const reference to the element at the given index
         * @throw out_of_range exception if the index lies not in the buffer range
         */
        const_reference &operator[](int idx) const noexcept;

        /** at
         * @brief access an element by index
         * @return const reference to the element at the given index
         * @throw out_of_range exception if the index lies not in the buffer range
         */
        const_reference &at(int idx) const;

    private:
        Alloc alloc_;
        T *buffer_;
        size_type head_;
        size_type tail_;
        size_type capacity_;
    };

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType>::reference
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator*() const
    {
        return circ_.buffer_[(circ_.head_ + static_cast<std::size_t>(static_cast<ptrdiff_t>(circ_.capacity_) + (offset_ % static_cast<ptrdiff_t>(circ_.capacity_))) % circ_.capacity_) % circ_.capacity_];
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType>::pointer
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator->()
    {
        return &circ_.buffer_[(circ_.head_ + static_cast<std::size_t>(static_cast<ptrdiff_t>(circ_.capacity_) + (offset_ % static_cast<ptrdiff_t>(circ_.capacity_))) % circ_.capacity_) % circ_.capacity_];
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType> &
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator++()
    {
        ++offset_;
        return *this;
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType>
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator++(int)
    {
        basic_iterator tmp = *this;
        ++offset_;
        return tmp;
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType> &
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator--()
    {
        --offset_;
        return *this;
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType>
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator--(int)
    {
        basic_iterator tmp = *this;
        --offset_;
        return tmp;
    }

    template <class T, class Alloc>
    template <class Container, class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<Container, ValueType>::difference_type
    circ_buffer<T, Alloc>::basic_iterator<Container, ValueType>::operator-(const basic_iterator<Container, ValueType> &it) const
    {
        return offset_ - it.offset_;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(const Alloc &a)
        : alloc_(a),
          buffer_(nullptr),
          head_(0),
          tail_(0),
          capacity_(0)
    {
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(size_type count, const Alloc &a)
        : alloc_(a),
          buffer_(alloc_.allocate(count)),
          head_(0),
          tail_(0),
          capacity_(count)
    {
    }

    template <class T, class Alloc>
    template <class Iter>
    circ_buffer<T, Alloc>::circ_buffer(Iter begin, Iter end, const Alloc &a)
        : alloc_(a),
          buffer_(alloc_.allocate(static_cast<std::size_t>(std::distance(begin, end)))),
          head_(0),
          tail_(0),
          capacity_(static_cast<std::size_t>(std::distance(begin, end)))
    {
        std::copy(begin, end, std::back_inserter(*this));
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(const circ_buffer &circ)
        : alloc_(circ.alloc_),
          buffer_(alloc_.allocate(circ.capacity_)),
          head_(circ.head_),
          tail_(circ.tail_),
          capacity_(circ.capacity_)
    {
        std::copy(circ.buffer_, circ.buffer_ + capacity_, buffer_);
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(circ_buffer &&circ) noexcept
        : alloc_(std::move(circ.alloc_)),
          buffer_(circ.buffer_),
          head_(circ.head_),
          tail_(circ.tail_),
          capacity_(circ.capacity_)
    {
        circ.begin_ = nullptr;
        circ.end_off_ = 0;
        circ.capacity_ = 0;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc> &circ_buffer<T, Alloc>::operator=(const circ_buffer &circ)
    {
        alloc_.deallocate(buffer_, capacity_);
        alloc_ = circ.alloc_;
        buffer_ = alloc_.allocate(circ.capacity_);
        head_ = circ.head_;
        tail_ = circ.tail_;
        capacity_ = circ.capacity_;
        std::copy(circ.buffer_, circ.buffer_ + capacity_, buffer_);
        return *this;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc> &circ_buffer<T, Alloc>::operator=(circ_buffer &&circ) noexcept
    {
        alloc_.deallocate(buffer_, capacity_);
        alloc_ = std::move(circ.alloc_);
        buffer_ = circ.buffer_;
        head_ = circ.head_;
        tail_ = circ.tail_;
        capacity_ = circ.capacity_;
        circ.buffer_ = nullptr;
        circ.head_ = 0;
        circ.tail_ = 0;
        circ.capacity_ = 0;
        return *this;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::~circ_buffer()
    {
        clear();
        alloc_.deallocate(buffer_, capacity_);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::iterator
    circ_buffer<T, Alloc>::begin() noexcept
    {
        return iterator(0, *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::iterator
    circ_buffer<T, Alloc>::end() noexcept
    {
        return iterator(static_cast<typename const_iterator::difference_type>(tail_ - head_), *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_iterator
    circ_buffer<T, Alloc>::cbegin() const noexcept
    {
        return const_iterator(0, *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_iterator
    circ_buffer<T, Alloc>::cend() const noexcept
    {
        return const_iterator(static_cast<typename const_iterator::difference_type>(tail_ - head_), *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reverse_iterator
    circ_buffer<T, Alloc>::rbegin() noexcept
    {
        return std::reverse_iterator<iterator>(begin());
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reverse_iterator
    circ_buffer<T, Alloc>::rend() noexcept
    {
        return std::reverse_iterator<iterator>(end());
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reverse_iterator
    circ_buffer<T, Alloc>::crbegin() const noexcept
    {
        return std::reverse_iterator<const_iterator>(cbegin());
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reverse_iterator
    circ_buffer<T, Alloc>::crend() const noexcept
    {
        return std::reverse_iterator<const_iterator>(cend());
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(value_type &&a)
    {
        if (tail_ - head_ == capacity_)
            pop_front();
        auto p = &buffer_[tail_ % capacity_];
        if (std::is_class<T>::value)
            std::allocator_traits<Alloc>::construct(alloc_, p, std::move(a));
        else
            *p = std::move(a);
        ++tail_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(const value_type &a)
    {
        if (tail_ - head_ == capacity_)
            pop_front();
        auto p = &buffer_[tail_ % capacity_];
        if (std::is_class<T>::value)
            std::allocator_traits<Alloc>::construct(alloc_, p, a);
        else
            *p = a;
        ++tail_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(value_type &&a)
    {
        if (tail_ - head_ == capacity_)
            pop_back();
        std::size_t new_head = head_ - 1;
        auto p = &buffer_[new_head % capacity_];
        if (std::is_class<T>::value)
            std::allocator_traits<Alloc>::construct(alloc_, p, std::move(a));
        else
            *p = std::move(a);
        head_ = new_head;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(const value_type &a)
    {
        if (tail_ - head_ == capacity_)
            pop_back();
        std::size_t new_head = head_ - 1;
        auto p = &buffer_[new_head % capacity_];
        if (std::is_class<T>::value)
            std::allocator_traits<Alloc>::construct(alloc_, p, a);
        else
            *p = a;
        head_ = new_head;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::size_type
    circ_buffer<T, Alloc>::size() const noexcept
    {
        return tail_ - head_;
    }

    template <class T, class Alloc>
    bool circ_buffer<T, Alloc>::empty() const noexcept
    {
        return (tail_ == head_);
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::pop_front()
    {
        if (size() > 0)
        {
            if (std::is_class<T>::value)
                buffer_[head_ % capacity_].~T();
            ++head_;
        }
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::pop_back()
    {
        if (size() > 0)
        {
            if (std::is_class<T>::value)
                buffer_[(tail_ - 1) % capacity_].~T();
            --tail_;
        }
    }

    template <class T, class Alloc>
    template <class... Args>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::emblace_front(Args &&...args)
    {
        if (tail_ - head_ == capacity_)
            pop_back();
        std::size_t new_head = head_ - 1;
        auto p = &buffer_[new_head % capacity_];
        std::allocator_traits<Alloc>::construct(alloc_, p, std::forward<Args>(args)...);
        head_ = new_head;
        return *p;
    }

    template <class T, class Alloc>
    template <class... Args>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::emblace_back(Args &&...args)
    {
        if (tail_ - head_ == capacity_)
            pop_front();
        auto p = &buffer_[tail_ % capacity_];
        std::allocator_traits<Alloc>::construct(alloc_, p, std::forward<Args>(args)...);
        ++tail_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::front()
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return buffer_[head_ % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::front() const
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return buffer_[head_ % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::back()
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return buffer_[(tail_ - 1) % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::back() const
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return buffer_[(tail_ - 1) % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::operator[](int idx) const noexcept
    {
        return buffer_[(head_ + idx) % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::at(int idx) const
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("circ_buffer: index out of range");
        return buffer_[(head_ + idx) % capacity_];
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::size_type circ_buffer<T, Alloc>::capacity() const noexcept
    {
        return capacity_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::clear() noexcept
    {
        while (!empty())
            pop_back();
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::set_capacity(size_type size)
    {
        auto new_buffer = alloc_.allocate(size);
        size_type offset = 0;
        for (auto iter = begin(); iter != end(); ++iter)
        {
            std::allocator_traits<Alloc>::construct(alloc_, new_buffer + offset, std::move(*iter));
            ++offset;
            offset %= capacity_;
        }
        clear();
        alloc_.deallocate(buffer_, capacity_);
        buffer_ = new_buffer;
        head_ = 0;
        tail_ = offset;
        capacity_ = size;
    }
} // namespace raphia
#endif
