#pragma once

#include "core/vec2.hpp"
#include "physics/shape.hpp"
#include <cstdint>
#include <concepts>

namespace physics{

struct BodyId {
    std::uint32_t index{};
    std::uint32_t generation{};
    friend bool operator==(BodyId, BodyId) = default;
};

struct RigidBody {
    core::Vec2 position{};
    core::Vec2 velocity{};

    float angle{0.0f};
    float angularVelocity{0.0f};

    float invMass{0.0f};
    float invInertia{0.0f};

    float friction{0.4f};
    float restitution{0.2f};

    Shape shape{Circle{}};

    [[nodiscard]] bool isStatic() const noexcept { return invMass == 0.0f; }
};

template<class T>
concept BodyLike = requires(T& body) {
    { body.position }        -> std::convertible_to<core::Vec2&>;
    { body.velocity }        -> std::convertible_to<core::Vec2&>;
    { body.angle }           -> std::convertible_to<float>;
    { body.angularVelocity } -> std::convertible_to<float>;
    { body.invMass }         -> std::convertible_to<float>;
};

template<BodyLike B>
void integrate(B& body, core::Vec2 gravity, float dt) noexcept {
    if (body.invMass == 0.0f) return;

    body.velocity += gravity * dt;
    body.position += body.velocity * dt;
    body.angle += body.angularVelocity * dt;
}

}