#pragma once

#include <ranges>
#include <utility>
#include <iterator>
#include <type_traits>

#include "Pulse/Types/Traits.hpp"
#include "Pulse/Types/Concepts.hpp"
#include "Pulse/Types/TypeUtils.hpp"

namespace Pulse::Views
{

    // TODO: Implement properly

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // Filter Iterator
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename FilterFunction>
    class FilterIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename Types::IteratorTraits<BaseIterator>::ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = typename Types::IteratorTraits<BaseIterator>::ValueType*;
        using reference = typename Types::IteratorTraits<BaseIterator>::ValueType&;

    public:
        // Constructors
        constexpr FilterIterator() = delete;
        constexpr FilterIterator(BaseIterator base, FilterFunction func);
        constexpr ~FilterIterator() = default;

        // Operators
        inline constexpr value_type operator * () const { return *m_Base; }

        constexpr FilterIterator& operator ++ ();
        constexpr FilterIterator operator ++ (int);

        constexpr bool operator == (const FilterIterator & other) const;
        constexpr bool operator != (const FilterIterator& other) const;

        constexpr bool operator == (const BaseIterator& other) const;
        constexpr bool operator != (const BaseIterator& other) const;

    private:
        void AdvanceToNextValid();

    private:
        BaseIterator m_Base;
        FilterFunction m_Func;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename FilterFunction>
    constexpr FilterIterator<BaseIterator, FilterFunction>::FilterIterator(BaseIterator base, FilterFunction func)
        : m_Base(base), m_Func(func)
    {
        AdvanceToNextValid();
    }

    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename FilterFunction>
    void FilterIterator<BaseIterator, FilterFunction>::AdvanceToNextValid()
    {
        while (m_Base != BaseIterator() && !m_Func(*m_Base))
            ++m_Base;
    }

    ///////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename FilterFunction>
    constexpr FilterIterator<BaseIterator, FilterFunction>& FilterIterator<BaseIterator, FilterFunction>::operator ++ ()
    {
        ++m_Base;
        AdvanceToNextValid();
        return *this;
    }

    template <typename BaseIterator, typename FilterFunction>
    constexpr FilterIterator<BaseIterator, FilterFunction> FilterIterator<BaseIterator, FilterFunction>::operator ++ (int)
    {
        FilterIterator<BaseIterator, FilterFunction> temp = *this;
        ++(*this);
        return temp;
    }

    template <typename BaseIterator, typename FilterFunction>
    constexpr bool FilterIterator<BaseIterator, FilterFunction>::operator == (const FilterIterator& other) const
    {
        return m_Base == other.m_Base;
    }

    template <typename BaseIterator, typename FilterFunction>
    constexpr bool FilterIterator<BaseIterator, FilterFunction>::operator != (const FilterIterator& other) const
    {
        return !(*this == other);
    }

    template <typename BaseIterator, typename FilterFunction>
    constexpr bool FilterIterator<BaseIterator, FilterFunction>::operator == (const BaseIterator& other) const
    {
        return m_Base == other;
    }

    template <typename BaseIterator, typename FilterFunction>
    constexpr bool FilterIterator<BaseIterator, FilterFunction>::operator != (const BaseIterator& other) const
    {
        return !(*this == other);
    }

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // Transform Iterator
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename TransformFunction>
    class TransformIterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename Types::IteratorTraits<BaseIterator>::ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = typename Types::IteratorTraits<BaseIterator>::ValueType*;
        using reference = typename Types::IteratorTraits<BaseIterator>::ValueType&;

    public:
        // Constructors
        constexpr TransformIterator() = delete;
        constexpr TransformIterator(BaseIterator base, TransformFunction func);
        constexpr ~TransformIterator() = default;

        // Operators
        inline constexpr value_type operator * () const { return m_Func(*m_Base); }

        constexpr TransformIterator& operator ++ ();
        constexpr TransformIterator operator ++ (int);

        constexpr bool operator == (const TransformIterator& other) const;
        constexpr bool operator != (const TransformIterator& other) const;

        constexpr bool operator == (const BaseIterator& other) const;
        constexpr bool operator != (const BaseIterator& other) const;

    private:
        BaseIterator m_Base;
        TransformFunction m_Func;
    };

    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename TransformFunction>
    constexpr TransformIterator<BaseIterator, TransformFunction>::TransformIterator(BaseIterator base, TransformFunction func)
        : m_Base(base), m_Func(func) {}

    ///////////////////////////////////////////////////////////
    // Operators
    ///////////////////////////////////////////////////////////
    template <typename BaseIterator, typename TransformFunction>
    constexpr TransformIterator<BaseIterator, TransformFunction>& TransformIterator<BaseIterator, TransformFunction>::operator ++ ()
    {
        ++m_Base;
        return *this;
    }

    template <typename BaseIterator, typename TransformFunction>
    constexpr TransformIterator<BaseIterator, TransformFunction> TransformIterator<BaseIterator, TransformFunction>::operator ++ (int)
    {
        TransformIterator<BaseIterator, TransformFunction> temp = *this;
        ++(*this);

        return temp;
    }

    template <typename BaseIterator, typename TransformFunction>
    constexpr bool TransformIterator<BaseIterator, TransformFunction>::operator == (const TransformIterator& other) const
    {
        return m_Base == other.m_Base;
    }

    template <typename BaseIterator, typename TransformFunction>
    constexpr bool TransformIterator<BaseIterator, TransformFunction>::operator != (const TransformIterator& other) const
    {
        return !(*this == other);
    }

    template <typename BaseIterator, typename TransformFunction>
    constexpr bool TransformIterator<BaseIterator, TransformFunction>::operator == (const BaseIterator& other) const
    {
        return m_Base == other;
    }

    template <typename BaseIterator, typename TransformFunction>
    constexpr bool TransformIterator<BaseIterator, TransformFunction>::operator != (const BaseIterator& other) const
    {
        return !(*this == other);
    }

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // Transform View
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    template <typename Range, typename TransformFunction>
    class TransformView
    {
    public:
        using Iterator = TransformIterator<typename Types::RangeTraits<Range>::Iterator, TransformFunction>;
        using Sentinel = typename Types::RangeTraits<Range>::Sentinel;

    public:
        // Constructors
        constexpr TransformView() = delete;
        constexpr TransformView(Range& range, TransformFunction func);
        constexpr ~TransformView() = default;

        // Methods
        constexpr Iterator begin();
        constexpr Sentinel end();

    private:
        Range& m_Range;
        TransformFunction m_Func;
    };

    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    // Filter View
    ///////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    template <typename Range, typename FilterFunction>
    class FilterView
    {
    public:
        using Iterator = FilterIterator<typename Types::RangeTraits<Range>::Iterator, FilterFunction>;
        using Sentinel = typename Types::RangeTraits<Range>::Sentinel;

    public:
        // Constructors
        constexpr FilterView() = delete;
        constexpr FilterView(Range& range, FilterFunction func);
        constexpr ~FilterView() = default;

        // Methods
        constexpr Iterator begin();
        constexpr Sentinel end();

    private:
        Range& m_Range;
        FilterFunction m_Func;
    };

    ///////////////////////////////////////////////////////////
    // Transform View
    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template <typename Range, typename TransformFunction>
    constexpr TransformView<Range, TransformFunction>::TransformView(Range& range, TransformFunction func)
        : m_Range(range), m_Func(func) {}

    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template <typename Range, typename TransformFunction>
    constexpr typename TransformView<Range, TransformFunction>::Iterator TransformView<Range, TransformFunction>::begin()
    {
        return Iterator(std::ranges::begin(m_Range), m_Func);
    }

    template <typename Range, typename TransformFunction>
    constexpr typename TransformView<Range, TransformFunction>::Sentinel TransformView<Range, TransformFunction>::end()
    {
        return std::ranges::end(m_Range);
    }

    ///////////////////////////////////////////////////////////
    // Filter View
    ///////////////////////////////////////////////////////////
    // Constructors
    ///////////////////////////////////////////////////////////
    template <typename Range, typename FilterFunction>
    constexpr FilterView<Range, FilterFunction>::FilterView(Range& range, FilterFunction func)
        : m_Range(range), m_Func(func) {}

    ///////////////////////////////////////////////////////////
    // Methods
    ///////////////////////////////////////////////////////////
    template <typename Range, typename FilterFunction>
    constexpr typename FilterView<Range, FilterFunction>::Iterator FilterView<Range, FilterFunction>::begin()
    {
        return Iterator(std::ranges::begin(m_Range), m_Func);
    }

    template <typename Range, typename FilterFunction>
    constexpr typename FilterView<Range, FilterFunction>::Sentinel FilterView<Range, FilterFunction>::end()
    {
        return std::ranges::end(m_Range);
    }

    ///////////////////////////////////////////////////////////
    // Combining FilterView and TransformView
    ///////////////////////////////////////////////////////////
    template <typename Range, typename FilterFunction, typename TransformFunction>
    TransformView<FilterView<Range, FilterFunction>, TransformFunction>
        make_transform_view_from_filter(FilterView<Range, FilterFunction> filter_view, TransformFunction transform_func)
    {
        return TransformView<FilterView<Range, FilterFunction>, TransformFunction>(filter_view, transform_func);
    }

    // Helper function to create a FilterView from a TransformView
    template <typename Range, typename TransformFunction, typename FilterFunction>
    FilterView<TransformView<Range, TransformFunction>, FilterFunction>
        make_filter_view_from_transform(TransformView<Range, TransformFunction> transform_view, FilterFunction filter_func)
    {
        return FilterView<TransformView<Range, TransformFunction>, FilterFunction>(transform_view, filter_func);
    }

}

