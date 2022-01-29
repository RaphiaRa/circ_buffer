#ifndef RAPHIA_CIRC_HPP
#define RAPHIA_CIRC_HPP
#include <cassert>
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
        template <class U>
        struct basic_iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = U;
            using pointer = U *;
            using reference = U &;
            using container = std::conditional_t<std::is_const<U>::value, const circ_buffer<T, Alloc> &, circ_buffer<T, Alloc> &>;

            reference operator*() const;
            pointer operator->();
            basic_iterator &operator++();
            basic_iterator operator++(int);
            typename basic_iterator::difference_type operator-(const basic_iterator &it) const;

            basic_iterator(difference_type offset, container &circ_buffer)
                : offset_(offset), circ_(circ_buffer) {}
            friend bool operator==(const basic_iterator &a, const basic_iterator &b) { return a.offset_ == b.offset_; };
            friend bool operator!=(const basic_iterator &a, const basic_iterator &b) { return a.offset_ != b.offset_; };

        private:
            difference_type offset_;
            container circ_;
        };

        using value_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = basic_iterator<T>;
        using const_iterator = basic_iterator<const T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = std::size_t;

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

        /** operator=
         * @brief copy operator
         */
        circ_buffer<T, Alloc> &operator=(const circ_buffer &);

        /** operator=
         * @brief move operator
         */
        circ_buffer<T, Alloc> &operator=(circ_buffer &&) noexcept;

        /** ~circ_buffer
         * @brief deconstructor
         */
        virtual ~circ_buffer();

        /** begin
         * @brief
         */
        iterator begin();

        /** end
         * @brief
         */
        iterator end();

        /** cbegin
         * @brief get contant iterator to the beginning of the circ buffer
         */
        const_iterator cbegin() const;

        /** cend
         * @brief get constatn iterator to the end of the circ buffer
         */
        const_iterator cend() const;

        /** push_back
         * @brief push a value to the end of the circular buffer, 
         * if the buffer is full, the first element will be overwritten
         * @param a rvalue
         */
        void push_back(value_type &&a);

        /** push_back
         * @brief push a value to the end of the circular buffer, 
         * if the buffer is full, the first element will be overwritten
         */
        void push_back(const value_type &a);

        /** push_front
         * @brief push a value to the front of the circular buffer, 
         * if the buffer is full, the last element will be overwritten
         */
        void push_front(value_type &&a);

        /** push_front
         * @brief push a value to the front of the circular buffer, 
         * if the buffer is full, the last element will be overwritten
         */
        void push_front(const value_type &a);

        /** size
         * @brief return the current count of elements in the buffer
         */
        size_type size();

        /** empty
         * @brief check whether the buffer is empty
         */
        bool empty();

        /** pop_front
         * @brief remove the front element from the buffer
         */
        void pop_front();

        /** pop_front
         * @brief remove the back element from the buffer
         */
        void pop_back();

        /** front
         * @brief access front element
         * @return reference to the front element
         */
        reference &front();

        /** front
         * @brief access front element
         * @return const reference to the front element
         */
        const_reference &front() const;

        /** back
         * @brief access back element
         * @return reference to the back element
         */
        reference &back();

        /** back
         * @brief access back element
         * @return const reference to the back element
         */
        const_reference &back() const;

        /** operator[]
         * @brief access element by index
         * @return reference to element at the given index
         */
        reference &operator[](int idx);

        /** operator[]
         * @brief access element by index
         * @return cosnt_reference to element at the given index
         */
        const_reference &operator[](int idx) const;

        /** operator[]
         * @brief access element by index
         * @return reference to element at the given index
         */
        reference &at(int idx);

        /** operator[]
         * @brief access element by index
         * @return const_reference to element at the given index
         */
        const_reference &at(int idx) const;

        /** capacity
         * @brief get the buffer capactiry
         */
        size_type capacity() const noexcept;

    private:
        Alloc alloc_;
        T *buffer_;
        T *begin_;
        size_type end_off_;
        size_type capacity_;
    };

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>::reference
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator*() const
    {
        auto p = circ_.begin_ + offset_;
        p = p < &circ_.buffer_[circ_.capacity_] ? p : p - circ_.capacity_;
        return *p;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>::pointer
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator->()
    {
        auto p = circ_.begin_ + offset_;
        p = p < &circ_.buffer_[circ_.capacity_] ? p : p - circ_.capacity_;
        return p;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType> &
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator++()
    {
        ++offset_;
        return *this;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator++(int)
    {
        basic_iterator tmp = *this;
        ++offset_;
        return tmp;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>::difference_type
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator-(const basic_iterator<ValueType> &it) const
    {
        return offset_ - it.offset_;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(const Alloc &a)
        : alloc_(a),
          buffer_(nullptr),
          begin_(nullptr),
          end_off_(0),
          capacity_(0)
    {
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(size_type count, const Alloc &a)
        : alloc_(a),
          buffer_(alloc_.allocate(count)),
          begin_(&buffer_[0]),
          end_off_(0),
          capacity_(count)
    {
    }

    template <class T, class Alloc>
    template <class Iter>
    circ_buffer<T, Alloc>::circ_buffer(Iter begin, Iter end, const Alloc &a)
        : alloc_(a),
          buffer_(alloc_.allocate(std::distance(begin, end))),
          begin_(&buffer_[0]),
          end_off_(0),
          capacity_(std::distance(begin, end))
    {
        std::copy(begin, end, std::back_inserter(*this));
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(const circ_buffer &circ)
        : alloc_(circ.alloc_),
          buffer_(alloc_.allocate(circ.capacity_)),
          begin_(circ.begin_),
          end_off_(circ.end_off_),
          capacity_(circ.capacity_)
    {
        std::copy(circ.buffer_, circ.buffer_ + capacity_, buffer_);
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(circ_buffer &&circ) noexcept
        : alloc_(std::move(circ.alloc_)),
          buffer_(circ.buffer_),
          begin_(circ.begin_),
          end_off_(circ.end_off_),
          capacity_(circ.capacity_)
    {
        circ.begin_ = nullptr;
        circ.end_off_ = 0;
        capacity_ = 0;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc> &circ_buffer<T, Alloc>::operator=(const circ_buffer &circ)
    {
        alloc_.deallocate(buffer_, capacity_);
        alloc_ = circ.alloc_;
        buffer_ = alloc_.allocate(circ.capacity_);
        begin_ = circ.begin_;
        end_off_ = circ.end_off_;
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
        begin_ = circ.begin_;
        end_off_ = circ.end_off_;
        capacity_ = circ.capacity_;
        circ.buffer_ = nullptr;
        circ.begin_ = nullptr;
        circ.end_off_ = 0;
        circ.capacity_ = 0;
        return *this;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::~circ_buffer()
    {
        while (!empty())
            pop_back();
        alloc_.deallocate(buffer_, capacity_);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::iterator
    circ_buffer<T, Alloc>::begin()
    {
        return iterator(0, *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::iterator
    circ_buffer<T, Alloc>::end()
    {
        return iterator(end_off_, *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_iterator
    circ_buffer<T, Alloc>::cbegin() const
    {
        return const_iterator(0, *this);
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_iterator
    circ_buffer<T, Alloc>::cend() const
    {
        return const_iterator(end_off_, *this);
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(value_type &&a)
    {
        if (end_off_ == capacity_)
            pop_front();
        auto p = begin_ + end_off_ < &buffer_[capacity_] ? begin_ + end_off_ : begin_ + end_off_ - capacity_;
        *p = std::move(a);
        ++end_off_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(const value_type &a)
    {
        if (end_off_ == capacity_)
            pop_front();
        auto p = begin_ + end_off_ < &buffer_[capacity_] ? begin_ + end_off_ : begin_ + end_off_ - capacity_;
        *p = a;
        ++end_off_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(value_type &&a)
    {
        if (end_off_ == capacity_)
            pop_back();
        if (begin_ == &buffer_[0])
            begin_ = &buffer_[capacity_];
        *(--begin_) = std::move(a);
        ++end_off_;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(const value_type &a)
    {
        if (end_off_ == capacity_)
            pop_back();
        if (begin_ == &buffer_[0])
            begin_ = &buffer_[capacity_];
        *(--begin_) = a;
        ++end_off_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::size_type
    circ_buffer<T, Alloc>::size()
    {
        return end_off_;
    }

    template <class T, class Alloc>
    bool circ_buffer<T, Alloc>::empty()
    {
        return size() == 0;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::pop_front()
    {
        if (size() > 0)
        {
            if (std::is_class<T>::value)
                *begin_ = T();
            if (++begin_ == &buffer_[capacity_])
                begin_ = &buffer_[0];
            --end_off_;
        }
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::pop_back()
    {
        if (size() > 0)
        {
            --end_off_;
            if (std::is_class<T>::value)
                back() = T();
        }
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::front()
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *begin_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::front() const
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *begin_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::back()
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        auto p = begin_ + end_off_ < &buffer_[capacity_] ? begin_ + end_off_ : begin_ + end_off_ - capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::back() const
    {
        if (empty())
            throw std::underflow_error("circ_buffer: tried to access empty container");
        auto p = begin_ + end_off_ < &buffer_[capacity_] ? begin_ + end_off_ : begin_ + end_off_ - capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::operator[](int idx)
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("circ_buffer: index out of range");
        T *p = begin_ + idx;
        if (p >= &buffer_[capacity_])
            p -= capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::operator[](int idx) const
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("circ_buffer: index out of range");
        T *p = begin_ + idx;
        if (p >= &buffer_[capacity_])
            p -= capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::at(int idx)
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("circ_buffer: index out of range");
        T *p = begin_ + idx;
        if (p >= &buffer_[capacity_])
            p -= capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::at(int idx) const
    {
        if (idx < 0 || idx >= size())
            throw std::out_of_range("circ_buffer: index out of range");
        T *p = begin_ + idx;
        if (p >= &buffer_[capacity_])
            p -= capacity_;
        return *p;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::size_type circ_buffer<T, Alloc>::capacity() const noexcept
    {
        return capacity_;
    }
} // namespace raphia
#endif