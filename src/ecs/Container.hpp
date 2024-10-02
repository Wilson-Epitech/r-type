/*
** R_TYPE PROJECT, 2024
** r-type
** File description:
** Container
*/

#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include <memory>
#include <optional>
#include <vector>

namespace ecs {
    template <typename Component> class SparseArray {
      public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>;
        using size_type = typename container_t::size_type;

        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        SparseArray() = default;

        SparseArray(SparseArray const &) = default;
        SparseArray(SparseArray &&) noexcept = default;
        ~SparseArray() = default;

        SparseArray &operator=(SparseArray const &) = default;
        SparseArray &operator=(SparseArray &&) noexcept = default;

        reference_type operator[](size_t idx) {
            if (idx >= size())
                _data.resize(idx + 1);
            return _data[idx];
        };

        const_reference_type operator[](size_t idx) const {
            if (idx >= size())
                _data.resize(idx + 1);
            return _data[idx];
        };

        iterator begin() {
            return _data.begin();
        };
        const_iterator begin() const {
            return _data.begin();
        };
        const_iterator cbegin() const {
            return _data.cbegin();
        };
        iterator end() {
            return _data.end();
        };
        const_iterator end() const {
            return _data.end();
        };
        const_iterator cend() const {
            return _data.cend();
        };

        size_type size() const {
            return _data.size();
        };

        reference_type insertAt(size_type pos, Component const &comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data[pos] = comp;
            return _data[pos];
        };

        reference_type insertAt(size_type pos, Component &&comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data[pos] = comp;
            return _data[pos];
        };

        template <class... Params> reference_type emplaceAt(size_type pos, Params &&...param) {
            if (pos >= size())
                _data.resize(pos + 1);
            _data.emplace(begin() + pos, (std::move(param), ...));
            return _data[pos];
        };

        void erase(size_type pos) {
            if (pos < size()) {
                _data[pos] = std::nullopt;
            }
        };

        size_type getIndex(value_type const &value) const {
            for (size_type i = 0; i < size(); i++) {
                if (std::addressof(_data[i]) == std::addressof(value))
                    return i;
            }
            return end();
        };

      private:
        container_t _data;
    };
}

#endif
