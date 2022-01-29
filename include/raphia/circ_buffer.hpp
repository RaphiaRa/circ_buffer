#ifndef RAPHIA_CIRC_HPP
#define RAPHIA_CIRC_HPP
#include <cassert>
#include <stdexcept>
#include <memory>

namespace raphia
{
    template <class T, class Alloc = std::allocator<T>>
    class circ_buffer
    {
    public:
        template <class U>
        struct basic_iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = U;
            using pointer = U *;
            using reference = U &;
            using container = std::conditional_t<std::is_const<U>::value, const circ_buffer<T, Alloc> &, circ_buffer<T, Alloc> &>;

            reference operator*() const { return *ptr_; }
            pointer operator->() { return ptr_; }
            basic_iterator &operator++();
            basic_iterator operator++(int);
            typename basic_iterator::difference_type operator-(const basic_iterator &it) const;

            basic_iterator(pointer ptr, container &circ_buffer)
                : ptr_(ptr), circ_(circ_buffer) {}
            friend bool operator==(const basic_iterator &a, const basic_iterator &b) { return a.ptr_ == b.ptr_; };
            friend bool operator!=(const basic_iterator &a, const basic_iterator &b) { return a.ptr_ != b.ptr_; };

        private:
            pointer ptr_;
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

        circ_buffer();
        explicit circ_buffer(size_type count);
        virtual ~circ_buffer();
        iterator begin() { return iterator(begin_, *this); }
        iterator end() { return iterator(end_, *this); }
        const_iterator cbegin() const { return const_iterator(begin_, *this); }
        const_iterator cend() const { return const_iterator(end_, *this); }
        void push_back(value_type &&a);
        void push_back(const value_type &a);
        void push_front(value_type &&a);
        void push_front(const value_type &a);
        size_type size();
        bool empty();
        void pop_front();
        void pop_back();
        reference &front();
        const_reference &front() const;
        reference &back();
        const_reference &back() const;
        reference &operator[](int idx);
        const_reference &operator[](int idx) const;
        reference &at(int idx);
        const_reference &at(int idx) const;
        size_type capacity() const noexcept;

    private:
        bool full_;
        Alloc alloc_;
        T *buffer_;
        T *begin_;
        T *end_;
        size_type capacity_;
    };

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType> &
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator++()
    {
        ++ptr_;
        if (ptr_ == &circ_.buffer_[circ_.capacity_])
            ptr_ = &circ_.buffer_[0];
        return *this;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator++(int)
    {
        basic_iterator tmp = *this;
        ptr_++;
        if (ptr_ == &circ_.buffer_[circ_.capacity_])
            ptr_ = &circ_.buffer_[0];
        return tmp;
    }

    template <class T, class Alloc>
    template <class ValueType>
    typename circ_buffer<T, Alloc>::template basic_iterator<ValueType>::difference_type
    circ_buffer<T, Alloc>::basic_iterator<ValueType>::operator-(const basic_iterator<ValueType> &it) const
    {
        if (ptr_ - it.ptr_ >= 0)
            return ptr_ - it.ptr_;
        else
            return &circ_.buffer_[circ_.capacity_] - &circ_.buffer_[0] + it.ptr_ - ptr_;
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer()
        : full_(true),
          alloc_(),
          buffer_(nullptr),
          begin_(nullptr),
          end_(nullptr),
          capacity_(0)
    {
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::circ_buffer(size_type count)
        : full_(false),
          alloc_(),
          buffer_(alloc_.allocate(count)),
          begin_(&buffer_[0]),
          end_(&buffer_[0]),
          capacity_(count)
    {
    }

    template <class T, class Alloc>
    circ_buffer<T, Alloc>::~circ_buffer()
    {
        while (!empty())
            pop_back();
        alloc_.deallocate(buffer_, capacity_);
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(value_type &&a)
    {
        if (full_)
            pop_front();
        *(end_++) = std::move(a);
        if (end_ == &buffer_[capacity_])
            end_ = &buffer_[0];
        if (end_ == begin_)
            full_ = true;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_back(const value_type &a)
    {
        if (full_)
            pop_front();
        *(end_++) = a;
        if (end_ == &buffer_[capacity_])
            end_ = &buffer_[0];
        if (end_ == begin_)
            full_ = true;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(value_type &&a)
    {
        if (full_)
            pop_back();
        if (begin_ == &buffer_[0])
            begin_ = &buffer_[capacity_];
        *(--begin_) = std::move(a);
        if (end_ == begin_)
            full_ = true;
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::push_front(const value_type &a)
    {
        if (full_)
            pop_back();
        if (begin_ == &buffer_[0])
            begin_ = &buffer_[capacity_];
        *(--begin_) = a;
        if (end_ == begin_)
            full_ = true;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::size_type
    circ_buffer<T, Alloc>::size()
    {
        return full_ ? capacity_ : std::distance(cbegin(), cend());
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
            if (full_)
                full_ = false;
        }
    }

    template <class T, class Alloc>
    void circ_buffer<T, Alloc>::pop_back()
    {
        if (size() > 0)
        {
            if (end_ == &buffer_[0])
                end_ = &buffer_[capacity_];
            --end_;
            if (std::is_class<T>::value)
                *end_ = T();
            if (full_)
                full_ = false;
        }
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::front()
    {
        if ((begin_ == end_) & !full_)
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *begin_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::front() const
    {
        if ((begin_ == end_) & !full_)
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *begin_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::reference circ_buffer<T, Alloc>::back()
    {
        if ((begin_ == end_) & !full_)
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *end_;
    }

    template <class T, class Alloc>
    typename circ_buffer<T, Alloc>::const_reference circ_buffer<T, Alloc>::back() const
    {
        if ((begin_ == end_) & !full_)
            throw std::underflow_error("circ_buffer: tried to access empty container");
        return *end_;
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