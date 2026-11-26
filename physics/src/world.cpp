#include "physics/world.hpp"

#include <utility>
#include <vector>

namespace physics {

struct World::Impl {
    core::Vec2                 gravity{};
    std::vector<RigidBody>     bodies;
    std::vector<std::uint32_t> generations;

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
    for (auto& body : impl_->bodies) {
        integrate(body, impl_->gravity, dt);
    }
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

} // namespace physics
