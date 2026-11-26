#include <raylib.h>

#include <core/config.hpp>
#include <physics/world.hpp>

#include <vector>

namespace {

constexpr int   kScreenW        = 1280;
constexpr int   kScreenH        = 720;
constexpr float kPixelsPerMeter = 64.0f;
constexpr float kGroundPx       = 60.0f;
constexpr float kFixedDt        = 1.0f / 120.0f;

// Физика: y растёт вверх. raylib: y растёт вниз. Переворот только здесь.
Vector2 toScreen(core::Vec2 p) noexcept {
    return Vector2{
        p.x * kPixelsPerMeter,
        (float(kScreenH) - kGroundPx) - p.y * kPixelsPerMeter
    };
}

void drawBody(const physics::World& world, physics::BodyId id) {
    const auto snapshot = world.state(id);
    const physics::Shape* shape = world.shape(id);
    if (!snapshot || !shape) return;

    const Vector2 center = toScreen(snapshot->position);

    if (const auto* circle = std::get_if<physics::Circle>(shape)) {
        DrawCircleV(center, circle->radius * kPixelsPerMeter, SKYBLUE);
    } else if (const auto* box = std::get_if<physics::Box>(shape)) {
        const float w = box->halfExtents.x * 2.0f * kPixelsPerMeter;
        const float h = box->halfExtents.y * 2.0f * kPixelsPerMeter;
        const float rotation = -snapshot->angle * RAD2DEG;   // угол тоже с минусом
        DrawRectanglePro(Rectangle{center.x, center.y, w, h},
                         Vector2{w / 2.0f, h / 2.0f}, rotation, ORANGE);
    }
}

} // namespace

int main() {
    InitWindow(kScreenW, kScreenH, "Termo Physics");
    SetTargetFPS(60);

    physics::World world{core::Vec2{0.0f, 0.0f}};

    std::vector<physics::BodyId> ids;

    physics::BodyDesc floorDesc{};
    floorDesc.position = {5.0f, -0.2f};
    floorDesc.mass     = 0.0f;                     // статическое тело
    floorDesc.shape    = physics::Box{{5.0f, 0.2f}};
    ids.push_back(world.addBody(floorDesc));

    for (int i = 0; i < 8; ++i) {
        physics::BodyDesc desc{};
        desc.position = {0.8f + float(i) * 1.0f, 8.0f};
        desc.shape    = physics::Circle{1.2f};
        if (i % 2 == 0){
            desc.velocity = {0.0f, 2.0f};
        } else {
            desc.velocity = {0.0f, -2.0f};
        }

        ids.push_back(world.addBody(desc));
    }

    float accumulator = 0.0f;

    while (!WindowShouldClose()) {
        accumulator += GetFrameTime();

        int substeps = 0;
        while (accumulator >= kFixedDt && substeps < 8) {
            world.step(kFixedDt);
            accumulator -= kFixedDt;
            ++substeps;
        }
        if (substeps == 8) accumulator = 0.0f;      // защита от «спирали смерти»

        BeginDrawing();
        ClearBackground(DARKGRAY);

        for (const auto id : ids) drawBody(world, id);

        for (const auto& c : world.contacts()) {
            const Vector2 p   = toScreen(c.point);
            const Vector2 tip = toScreen(c.point + c.normal * 0.5f);
            DrawCircleV(p, 6.0f, RED);
            DrawLineEx(p, tip, 3.0f, GREEN);
        }

        DrawText(TextFormat("bodies: %d  contacts: %d",
                            int(world.bodyCount()), int(world.contacts().size())),
                20, 20, 24, RAYWHITE);

        EndDrawing();



    }

    CloseWindow();
    return 0;
}
