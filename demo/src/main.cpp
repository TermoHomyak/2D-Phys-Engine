#include <raylib.h>
#include <core/vec2.hpp>

int main() {
    InitWindow(1280, 720, "Termo Physics");
    SetTargetFPS(60);

    constexpr core::Vec2 v{3.0f, 4.0f};
    static_assert(core::dot(v, v) > 0.0f, "dot должен считаться на этапе компиляции");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText(TextFormat("|v| = %.2f", v.length()), 20, 20, 24, RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
