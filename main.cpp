#include <raylib.h>

namespace ECS
{
}

int main()
{
    InitWindow(1000, 700, "ecs");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
