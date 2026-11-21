#ifndef ECS_ECS_H
#define ECS_ECS_H

#include <cstdint>
#include <optional>
#include <raylib.h>
#include <unordered_map>

namespace ECS
{
    using Entity = std::uint32_t;

    struct Component
    {
    };

    struct Transform : public Component
    {
        Vector2 position{};
    };

    struct Circle : public Component
    {
        float radius{};
        Color color{};
    };

    struct Movement : public Component
    {
        Vector2 direction{};
    };

    template <typename T>
    concept ComponentConcept = std::derived_from<T, Component>;

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

        template <ComponentConcept ComponentType, typename... Args>
        ComponentType& addComponent(Entity entity, Args&&... args)
        {
            auto& storage{getStorage<ComponentType>()};
            return storage.addComponent(entity, std::forward<Args>(args)...);
        }

        template <ComponentConcept ComponentType>
        std::optional<std::reference_wrapper<ComponentType>> getComponent(Entity entity)
        {
            auto& storage{getStorage<ComponentType>()};
            return storage.getComponent(entity);
        }

        template <ComponentConcept ComponentType>
        auto& getComponentMap()
        {
            return getStorage<ComponentType>().getMap();
        }

    private:
        World() = default;
        World(const World&) = delete;
        World(World&&) = delete;

        Entity m_nextEntity{1};

        template <ComponentConcept ComponentType>
        class Storage
        {
        public:
            template <typename... Args>
            ComponentType& addComponent(Entity entity, Args&&... args)
            {
                auto [it, inserted]{m_storage.emplace(entity, ComponentType{{}, std::forward<Args>(args)...})};
                return it->second;
            }

            std::optional<std::reference_wrapper<ComponentType>> getComponent(Entity entity)
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
            std::unordered_map<Entity, ComponentType> m_storage{};
        };

        template <ComponentConcept ComponentType>
        Storage<ComponentType>& getStorage()
        {
            static Storage<ComponentType> storage{};
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
