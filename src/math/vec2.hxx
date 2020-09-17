#ifndef JARALYN_VEC2_HXX
#define JARALYN_VEC2_HXX

template<typename t>
struct j_vec2 {
    t x { 0 };
    t y { 0 };

    /**
     * @brief Constructs a null-vector
     */
    constexpr j_vec2() = default;

    /**
     * @brief Copy-constructs a vector from another vector
     */
    constexpr j_vec2(const j_vec2<t>& v) noexcept {
        x = v.x;
        y = v.y;
    }

    /**
     * @brief Copy-constructs a vector from another vector
     */
    template<typename u>
    constexpr j_vec2(const u& v) noexcept {
        x = static_cast<t> (v.x);
        y = static_cast<t> (v.y);
    }

    /**
     * @brief Constructs a vector from two scalars
     */
    constexpr j_vec2(t x, t y) noexcept {
        this->x = x;
        this->y = y;
    }

    /**
     * @brief Adds the given vector
     */
    constexpr inline void operator+=(const j_vec2<t>& rhs) noexcept {
        this->x += rhs.x;
        this->y += rhs.y;
    }

    /**
     * @brief Adds the given scalar
     */
    constexpr inline void operator+=(const t scalar) noexcept {
        this->x += scalar;
        this->y += scalar;
    }

    /**
     * @brief Subtracts the given vector
     */
    constexpr inline void operator-=(const j_vec2<t>& rhs) noexcept {
        this->x -= rhs.x;
        this->y -= rhs.y;
    }

    /**
     * @brief Subtracts the given scalar
     */
    constexpr inline void operator-=(const t scalar) noexcept {
        this->x -= scalar;
        this->y -= scalar;
    }

    /**
     * @brief Multiplies the vector parts by the given scalar
     */
    constexpr inline void operator*=(const t scalar) noexcept {
        this->x *= scalar;
        this->y *= scalar;
    }

    /**
     * @brief Divides the vector parts by the given scalar
     */
    constexpr inline void operator/=(const t scalar) noexcept {
        this->x /= scalar;
        this->y /= scalar;
    }
};

/**
 * @brief Compares two vectors for equality
 *
 * Unreliable on floats
 */
template<typename t>
[[nodiscard]] constexpr inline bool operator==(const j_vec2<t>& lhs, const j_vec2<t>& rhs) noexcept {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/**
 * @brief Returns a new negated version of the given vector
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator-(const j_vec2<t>& v) noexcept {
    return j_vec2<t>(-v.x, -v.y);
}

/**
 * @brief Subtracts a scalar from all vector parts
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator-(const j_vec2<t>& v, t scalar) noexcept {
    return j_vec2<t>(v.x - scalar, v.y - scalar);
}

/**
 * @brief Subtracts two given vectors
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator-(const j_vec2<t>& lhs, const j_vec2<t>& rhs) noexcept {
    return j_vec2<t>(lhs.x - rhs.x, lhs.y - rhs.y);
}

/**
 * @brief Adds two given vectors
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator+(const j_vec2<t>& lhs, const j_vec2<t>& rhs) noexcept {
    return j_vec2<t>(lhs.x + rhs.x, lhs.y + rhs.y);
}

/**
 * @brief Adds a scalar to all vector parts
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator+(const j_vec2<t>& v, t scalar) noexcept {
    return j_vec2<t>(v.x + scalar, v.y + scalar);
}

/**
 * @brief Divides all vector parts by the given scalar
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator/(const j_vec2<t>& v, t scalar) {
    return j_vec2<t>(v.x / scalar, v.y / scalar);
}

/**
 * @brief Multiplies all vector parts by the given scalar
 */
template<typename t>
[[nodiscard]] constexpr inline j_vec2<t> operator*(const j_vec2<t>& v, t scalar) noexcept {
    return j_vec2<t>(v.x * scalar, v.y * scalar);
}

#endif
