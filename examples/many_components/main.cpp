#include <chrono>
#include <iostream>
#include <ECS/ecs.hpp>

static volatile float sum{};

static void benchmark_system()
{
    for (auto& world{ECS::World::getInstance()}; const auto& entity : world.getStorage<ECS::Circle>().getEntities())
    {
        const auto* circle_component{world.getComponent<ECS::Circle>(entity)};
        auto* transformComponent{world.getComponent<ECS::Transform>(entity)};

        auto [x,y] = transformComponent->position;
        sum += x + y;
        sum += circle_component->radius;
    }
}

int main()
{
    auto& world{ECS::World::getInstance()};

    const auto start{std::chrono::high_resolution_clock::now()};

    for (size_t i{}; i < 10'000; ++i)
    {
        const auto circle{world.createEntity()};
        world.addComponent<ECS::Transform>(circle, Vector2(2, 2));
        world.addComponent<ECS::Circle>(circle, 100.0f, RED);
    }

    for (size_t i{}; i < 1'000; ++i)
    {
        benchmark_system();
    }

    const auto end{std::chrono::high_resolution_clock::now()};

    const auto duration{std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()};
    std::cout << "Elapsed time: " << duration << " ms\n";

    return 0;
}

// std::unordered_map Elapsed time: 2905 ms
// SparseSet Elapsed time: 343 ms
