#pragma once

#include <vector>
#include <optional>
#include <any>
#include <iostream>

template<typename T> // You can also mirror the definition of std :: vector ,that takes an additional allocator .
class sparseArray {
    public :
        using value_type = std::optional<T>; // optional T(component) type
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>; // optionally add your allocator template here
        using size_type = typename container_t::size_type;
        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

    public :
        sparseArray(); // You can add more constructors .
        sparseArray(sparseArray const &); // copy constructor
        sparseArray(sparseArray &&) noexcept; // move constructor
        ~sparseArray();

        sparseArray &operator=(sparseArray const &); // copy assignment operator
        sparseArray &operator=(sparseArray &&) noexcept; // move assignment operator
        reference_type operator[](size_t idx);

        const_reference_type operator[](size_t idx) const;

        iterator begin();

        const_iterator begin() const;

        const_iterator cbegin() const;

        iterator end();

        const_iterator end() const;

        const_iterator cend() const;

        size_type size() const;

        /**
         *
         * @return numbers of activated elements
         */
        size_type numberOfElements() const;

        /**
         *
         * @param pos index
         * @return reference to new instance
         */
        reference_type insert_at(size_type pos, const value_type &);

        /**
         *
         * @param pos index
         * @return reference to new instance
         */
        reference_type insert_at(size_type pos, value_type &&);

        template<class... Params>
        reference_type emplace_back(Params &&...);

        template<class... Params>
        reference_type emplace_at(size_type pos, Params &&...);// allocator_traits//decltype specifier
        void erase(size_type pos);

        /**
         *
         * @return get the index thanks to the reference of an element contained in it
         */
        size_type get_index(value_type const &) const;

        /**
         * desactivate a component at pos
         * @param pos
         */
        void deactivateComponent(size_type pos);

    private :
        container_t _data;
};

#include "Components/Components.hpp"
#include "sparseArray.impl"
