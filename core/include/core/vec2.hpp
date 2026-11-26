#pragma once

#include <cmath>
#include <type_traits>

namespace core {

    struct Vec2 {
        float x{};
        float y{};

        [[nodiscard]] constexpr Vec2 operator+(Vec2 o) const noexcept { return {x + o.x, y + o.y}; }
        [[nodiscard]] constexpr Vec2 operator-(Vec2 o) const noexcept { return {x - o.x, y - o.y}; }
        [[nodiscard]] constexpr Vec2 operator*(float s) const noexcept { return {x * s, y * s}; }
        [[nodiscard]] constexpr Vec2 operator-() const noexcept { return {-x, -y}; }
        [[nodiscard]] constexpr bool operator==(Vec2 o) const noexcept { return (x == o.x) && (y == o.y); }

        constexpr Vec2& operator+=(Vec2 o) noexcept { x += o.x; y += o.y; return *this; }
        constexpr Vec2& operator-=(Vec2 o) noexcept { x -= o.x; y -= o.y; return *this; }
        constexpr Vec2& operator*=(float s) noexcept { x *= s; y *= s; return *this; }

        [[nodiscard]] constexpr float lengthSq() const noexcept { return x * x + y * y; }
        [[nodiscard]] float length() const noexcept { return std::sqrt(lengthSq()); }

        [[nodiscard]] Vec2 normalized() noexcept { 
            const float len = length();
            if (len <= 0.0f) {
                return {0.0f, 0.0f};
            }
            const float inv = 1.0f / len;
            return {x * inv, y * inv};
        }

        [[nodiscard]] constexpr Vec2 perp() const noexcept { return {-y, x};}
    };

    [[nodiscard]] constexpr float dot(Vec2 a, Vec2 b) noexcept { return a.x * b.x + a.y * b.y; }
    [[nodiscard]] constexpr float cross(Vec2 a, Vec2 b) noexcept { return a.x * b.y - a.y * b.x; }
    [[nodiscard]] constexpr Vec2 operator*(float s, Vec2 v) noexcept { return {v.x * s, v.y * s}; }

    
    static_assert(std::is_trivially_copyable_v<Vec2>);
    static_assert(Vec2{1.0f, 2.0f}.perp() == Vec2{-2.0f, 1.0f});
    static_assert(2.0f * Vec2{1.0f, 2.0f} == Vec2{2.0f, 4.0f});


}
