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

    constexpr Vec2& operator+=(Vec2 o) noexcept { x += o.x; y += o.y; return *this; }
    constexpr Vec2& operator-=(Vec2 o) noexcept { x -= o.x; y -= o.y; return *this; }
    constexpr Vec2& operator*=(float s) noexcept { x *= s; y *= s; return *this; }

    [[nodiscard]] constexpr float lengthSq() const noexcept { return x * x + y * y; }

    [[nodiscard]] float length() const noexcept { return std::sqrt(lengthSq()); }
};

[[nodiscard]] constexpr float dot(Vec2 a, Vec2 b) noexcept { return a.x * b.x + a.y * b.y; }
[[nodiscard]] constexpr float cross(Vec2 a, Vec2 b) noexcept { return a.x * b.y - a.y * b.x; }

static_assert(std::is_trivially_copyable_v<Vec2>);

}
