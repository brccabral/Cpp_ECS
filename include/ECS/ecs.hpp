#ifndef ECS_ECS_H
#define ECS_ECS_H

#include <cstdint>
#include <optional>
#include <raylib.h>
#include <unordered_map>

namespace ECS
{
    using Entity = std::uint32_t;

    struct Transform
    {
        Vector2 position{};
    };

    struct Circle
    {
        float radius{};
        Color color{};
    };

    struct Movement
    {
        Vector2 direction{};
    };

    template <typename T>
    concept ComponentConcept = std::is_same_v<T, Transform> || std::is_same_v<T, Circle> || std::is_same_v<T, Movement>;

    class World
    {
    public:
        static World& getInstance()
        {
            static World world;
            return world;
        }

        Entity createEntity()
        {
            return m_nextEntity++;
        }

        template <ComponentConcept Component, typename... Args>
        Component& addComponent(Entity entity, Args&&... args)
        {
            auto& storage{getStorage<Component>()};
            return storage.addComponent(entity, std::forward<Args>(args)...);
        }

        template <ComponentConcept Component>
        std::optional<std::reference_wrapper<Component>> getComponent(Entity entity)
        {
            auto& storage{getStorage<Component>()};
            return storage.getComponent(entity);
        }

        template <ComponentConcept Component>
        auto& getComponentMap()
        {
            return getStorage<Component>().getMap();
        }

    private:
        World() = default;
        World(const World&) = delete;
        World(World&&) = delete;

        Entity m_nextEntity{1};

        template <ComponentConcept Component>
        class Storage
        {
        public:
            template <typename... Args>
            Component& addComponent(Entity entity, Args&&... args)
            {
                auto [it, inserted]{m_storage.emplace(entity, Component{std::forward<Args>(args)...})};
                return it->second;
            }

            std::optional<std::reference_wrapper<Component>> getComponent(Entity entity)
            {
                auto it{m_storage.find(entity)};
                if (it == m_storage.end())
                {
                    return std::nullopt;
                }
                return it->second;
            }

            auto& getMap()
            {
                return m_storage;
            }

        private:
            std::unordered_map<Entity, Component> m_storage{};
        };

        template <ComponentConcept Component>
        Storage<Component>& getStorage()
        {
            static Storage<Component> storage{};
            return storage;
        }
    };

    // Systems
#pragma region Systems
    static void circle_renderer_system()
    {
        for (auto& world{World::getInstance()}; const auto& [entity, circle_component] : world.getComponentMap<
                 Circle>())
        {
            auto transform_component{world.getComponent<Transform>(entity)};
            if (!transform_component.has_value())
            {
                continue;
            }
            auto [x, y] = transform_component->get().position;
            DrawCircle(x, y, circle_component.radius, circle_component.color);
        }
    }
#pragma endregion
}

#endif //ECS_ECS_H
