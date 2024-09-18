/*
** R_TYPE PROJECT, 2024
** bs
** File description:
** Container
*/

#ifndef CONTAINER_HPP_
    #define CONTAINER_HPP_

    #include <vector>
    #include <memory>
    #include <optional>

template <typename Component> // You can also mirror the definition of std::vector , that takes an additional allocator .

class sparse_array {
    public:
        using value_type = std::optional<Component>;
        using reference_type = value_type &;
        using const_reference_type = value_type const &;
        using container_t = std::vector<value_type>; // optionally add your allocator template here .
        using size_type = typename container_t::size_type;

        using iterator = typename container_t::iterator;
        using const_iterator = typename container_t::const_iterator;

        sparse_array() = default; // You can add more constructors .

        sparse_array(sparse_array const &) = default; // copy constructor
        sparse_array(sparse_array &&) noexcept = default; // move constructor
        ~sparse_array() = default;

        sparse_array & operator=(sparse_array const &) = default; // copy assignment operator
        sparse_array & operator=(sparse_array &&) noexcept = default; // move assignment operator

        reference_type operator[](size_t idx) {
            if (idx < size())
                return _data[idx];
            return std::nullopt;
        };

        const_reference_type operator[](size_t idx) const {
            if (idx < size())
                return _data[idx];
            return std::nullopt;
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

        reference_type insert_at(size_type pos, Component const &comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            erase(pos);
            _data.insert(begin() + pos, std::move(comp));
            return _data[pos];
        };

        reference_type insert_at(size_type pos, Component &&comp) {
            if (pos >= size())
                _data.resize(pos + 1);
            erase(pos);
            _data.insert(begin() + pos, std::move(comp));
            return _data[pos];
        };

        template <class ... Params>
        reference_type emplace_at(size_type pos, Params &&...param) {
            if (pos >= size())
                _data.resize(pos + 1);
            destroy(_data[pos]);
            _data.emplace(begin() + pos, (std::move(param), ...));
            return _data[pos];
        };

        void erase(size_type pos) {
            if (pos < size())
                _data.erase(begin() + pos);
        };

        size_type get_index(value_type const &value) const {
            for (size_type i = cbegin(); i < cend(); i++) {
                if (std::addressof(_data[i]) == std::addressof(value))
                    return i;
            }
            return cend();
        };

    private:
        container_t _data;
};

#endif
