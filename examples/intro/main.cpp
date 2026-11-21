#include <ECS/ecs.hpp>

int main()
{
    InitWindow(1000, 700, "ecs");
    SetTargetFPS(60);

    Vector2 middle_screen{.x = GetScreenWidth() / 2.0f, .y = GetScreenHeight() / 2.0f};

    auto& world{ECS::World::getInstance()};

    auto bigRedCircle{world.createEntity()};
    world.addComponent<ECS::Transform>(bigRedCircle, Vector2(middle_screen));
    world.addComponent<ECS::Circle>(bigRedCircle, 100.0f, RED);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        ECS::circle_renderer_system();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
