#include "physics/world.hpp"

#include <utility>
#include <vector>

namespace physics {

struct World::Impl {
    core::Vec2                 gravity{};
    std::vector<RigidBody>     bodies;
    std::vector<std::uint32_t> generations;
    std::vector<Contact> contacts;

    explicit Impl(core::Vec2 g) : gravity{g} {
        bodies.reserve(256);
        generations.reserve(256);
    }

    [[nodiscard]] bool valid(BodyId id) const noexcept {
        return id.index < bodies.size() && generations[id.index] == id.generation;
    }
};

World::World(core::Vec2 gravity) : impl_{std::make_unique<Impl>(gravity)} {}

World::~World() = default;

World::World(World&& other) noexcept            = default;
World& World::operator=(World&& other) noexcept = default;

BodyId World::addBody(const BodyDesc& desc) {
    RigidBody body{};
    body.position    = desc.position;
    body.shape       = desc.shape;
    body.friction    = desc.friction;
    body.restitution = desc.restitution;
    body.velocity    = desc.velocity;

    if (desc.mass > 0.0f) {
        body.invMass = 1.0f / desc.mass;
        const float inertia = momentOfInertia(desc.shape, desc.mass);
        body.invInertia = inertia > 0.0f ? 1.0f / inertia : 0.0f;
    }

    impl_->bodies.push_back(body);
    impl_->generations.push_back(1u);

    return BodyId{static_cast<std::uint32_t>(impl_->bodies.size() - 1u), 1u};
}

void World::step(float dt) {
    for (auto& body : impl_->bodies) integrateVelocity(body, impl_->gravity, dt);
    impl_->contacts.clear();
    auto& bodies = impl_->bodies;

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies.size(); ++j) {
            if (bodies[i].isStatic() && bodies[j].isStatic()) continue;

            if (auto contact = collideCircles(BodyId{static_cast<std::uint32_t>(i), 1u}, bodies[i],
                                            BodyId{static_cast<std::uint32_t>(j), 1u}, bodies[j])) {
                impl_->contacts.push_back(*contact);
            }
        }
    }


    for (auto& body : impl_->bodies) integratePosition(body, dt);
}

std::optional<BodySnapshot> World::state(BodyId id) const {
    if (!impl_->valid(id)) return std::nullopt;
    const RigidBody& body = impl_->bodies[id.index];
    return BodySnapshot{body.position, body.angle};
}

const Shape* World::shape(BodyId id) const {
    if (!impl_->valid(id)) return nullptr;
    return &impl_->bodies[id.index].shape;
}

std::size_t World::bodyCount() const noexcept {
    return impl_->bodies.size();
}

std::span<const Contact> World::contacts() const noexcept {
    return impl_->contacts;
}


} // namespace physics
