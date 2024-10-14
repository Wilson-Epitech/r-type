/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Container
*/

/**
 * @file Container.hpp
 * @brief Defines the `SparseArray` template class used for managing components in the ECS.
 * This file contains the `SparseArray` template class, which provides a flexible container
 * for storing and managing optional components in the ECS (Entity Component System). It allows
 * dynamic resizing, insertion, and deletion of components.
 */

#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include <memory>
#include <optional>
#include <vector>

namespace ecs {

    /**
     * @class SparseArray
     * @brief A dynamic array for managing optional components in the ECS.
     * The `SparseArray` is a template class that manages components in a dynamic array.
     * Components are stored in `std::optional` objects, allowing the array to handle absent
     * or uninitialized components. The array supports dynamic resizing and insertion at arbitrary
     * positions.
     * @tparam Component The type of component managed by the sparse array.
     */
    template <typename Component> class SparseArray {
      public:
        using value_type = std::optional<Component>;       ///< Alias for optional component type.
        using reference_type = value_type &;               ///< Alias for a reference to an optional component.
        using const_reference_type = value_type const &;   ///< Alias for a const reference to an optional component.
        using container_t = std::vector<value_type>;       ///< Alias for the container type (vector of optional components).
        using size_type = typename container_t::size_type; ///< Alias for size type of the container.

        using iterator = typename container_t::iterator;             ///< Iterator type for the container.
        using const_iterator = typename container_t::const_iterator; ///< Const iterator type for the container.

        SparseArray() = default;

        SparseArray(SparseArray const &) = default;
        SparseArray(SparseArray &&) noexcept = default;
        ~SparseArray() = default;

        SparseArray &operator=(SparseArray const &) = default;
        SparseArray &operator=(SparseArray &&) noexcept = default;

        /**
         * @brief Accesses a component at the given index.
         * If the index is out of bounds, the container is resized to accommodate the new index.
         * @param idx The index to access.
         * @return reference_type A reference to the optional component at the specified index.
         */
        reference_type operator[](size_t idx) {
            if (idx >= size())
                _data.resize(idx + 1);
            return _data[idx];
        };

        /**
         * @brief Accesses a const component at the given index.
         * If the index is out of bounds, the container is resized to accommodate the new index.
         * @param idx The index to access.
         * @return const_reference_type A const reference to the optional component at the specified index.
         */
        const_reference_type operator[](size_t idx) const {
            if (idx >= size())
                _data.resize(idx + 1);
            return _data[idx];
        };

        /**
         * @brief Returns an iterator to the beginning of the container.
         * @return iterator Iterator to the first component in the container.
         */
        iterator begin() {
            return _data.begin();
        };

        /**
         * @brief Returns a const iterator to the beginning of the container.
         * @return const_iterator Const iterator to the first component in the container.
         */
        const_iterator begin() const {
            return _data.begin();
        };

        const_iterator cbegin() const {
            return _data.cbegin();
        };

        /**
         * @brief Returns an iterator to the end of the container.
         * @return iterator Iterator to the element following the last component.
         */
        iterator end() {
            return _data.end();
        };

        /**
         * @brief Returns a const iterator to the end of the container.
         * @return const_iterator Const iterator to the element following the last component.
         */
        const_iterator end() const {
            return _data.end();
        };

        const_iterator cend() const {
            return _data.cend();
        };

        /**
         * @brief Returns the number of elements in the container.
         * @return size_type The number of elements in the container.
         */
        size_type size() const {
            return _data.size();
        };

        /**
         * @brief Inserts a component at the specified position.
         * Resizes the container if necessary and inserts the given component at the specified index.
         * @param pos The position where the component will be inserted.
         * @param comp The component to insert.
         * @return reference_type A reference to the inserted component.
         */
        reference_type insertAt(size_type pos, Component const &comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data[pos] = comp;
            return _data[pos];
        };

        /**
         * @brief Inserts a component (rvalue) at the specified position.
         * Resizes the container if necessary and inserts the given component at the specified index.
         * @param pos The position where the component will be inserted.
         * @param comp The component to insert (as an rvalue).
         * @return reference_type A reference to the inserted component.
         */
        reference_type insertAt(size_type pos, Component &&comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data[pos] = comp;
            return _data[pos];
        };

        /**
         * @brief Emplaces a new component at the specified position.
         * Constructs a new component in-place at the specified position.
         * @tparam Params The types of the parameters used to construct the component.
         * @param pos The position where the component will be emplaced.
         * @param param The parameters to pass to the component's constructor.
         * @return reference_type A reference to the newly constructed component.
         */
        template <class... Params> reference_type emplaceAt(size_type pos, Params &&...param) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data.emplace(begin() + pos, (std::move(param), ...));
            return _data[pos];
        };

        /**
         * @brief Erases a component at the specified position.
         * The component at the given position is reset to `std::nullopt`, effectively removing it.
         * @param pos The position of the component to erase.
         */
        void erase(size_type pos) {
            if (pos < size()) {
                _data[pos] = std::nullopt;
            }
        };

        /**
         * @brief Finds the index of a given component value.
         * @param value The component value to find.
         * @return size_type The index of the component, or the end iterator if not found.
         */
        size_type getIndex(value_type const &value) const {
            for (size_type i = 0; i < size(); i++) {
                if (std::addressof(_data[i]) == std::addressof(value))
                    return i;
            }
            return end();
        };

      private:
        container_t _data; ///< The container that holds the optional components.
    };
}

#endif
