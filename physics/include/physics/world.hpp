#pragma once

#include "core/vec2.hpp"
#include "physics/shape.hpp"
#include "physics/body.hpp"
#include <optional>
#include "core/config.hpp"
#include <cstddef>
#include <memory>
#include <span>
#include "physics/contact.hpp"

namespace physics{

    struct BodyDesc {
        core::Vec2 position{};
        float      mass{1.0f};
        Shape      shape{Circle{}};
        float      friction{0.4f};
        float      restitution{0.2f};
        core::Vec2 velocity{};
    };

    struct BodySnapshot {
        core::Vec2 position{};
        float      angle{0.0f};
    };

    class World {
    public:
        explicit World(core::Vec2 gravity = {0.0f, -core::Defaults::gravity});
        ~World();

        World(const World&)            = delete;
        World& operator=(const World&) = delete;
        World(World&&) noexcept;
        World& operator=(World&&) noexcept;

        BodyId addBody(const BodyDesc& desc);
        void   step(float dt);

        [[nodiscard]] std::optional<BodySnapshot> state(BodyId id) const;
        [[nodiscard]] const Shape* shape(BodyId id) const;
        [[nodiscard]] std::size_t  bodyCount() const noexcept;
        [[nodiscard]] std::span<const Contact> contacts() const noexcept;


    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}