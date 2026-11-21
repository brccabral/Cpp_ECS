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

    struct Transform : Component
    {
        Vector2 position{};
    };

    struct Circle : Component
    {
        float radius{};
        Color color{};
    };

    struct Movement : Component
    {
        Vector2 direction{};
    };

    template <typename T>
    concept ComponentConcept = std::derived_from<T, Component>;

    template <ComponentConcept ComponentType>
    class SparseSet
    {
    public:
        // map.emplace
        ComponentType& insert(const Entity key, const ComponentType& component)
        {
            // resize vectors if needed
            ensure_sparse_size(key);

            // ComponentType for thet Entity does not exist yet
            if (sparse[key] == INVALID)
            {
                sparse[key] = dense.size();
                dense_keys.push_back(key);
                dense.push_back(component);
            }
            // ComponentType for thet Entity exists, update it
            else
            {
                dense[sparse[key]] = component;
            }

            return dense[sparse[key]];
        }

        ComponentType* get(const Entity key)
        {
            if (key >= sparse.size())
            {
                return nullptr;
            }
            auto index = sparse[key];
            if (index == INVALID)
            {
                return nullptr;
            }

            return &dense[index];
        }

        const auto& getEntities() const
        {
            return dense_keys;
        }

    private:
        std::vector<std::size_t> sparse{}; // indexes for the dense_keys/dense vectors
        std::vector<std::size_t> dense_keys{}; // indexes for the sparse vector (entity id)
        std::vector<ComponentType> dense; // component storage
        static constexpr std::size_t INVALID{std::numeric_limits<std::size_t>::max()};

        // resize vectors if needed
        void ensure_sparse_size(const Entity key)
        {
            if (key >= sparse.size())
            {
                sparse.resize(key + 1, INVALID);
            }
        }
    };

    template <ComponentConcept ComponentType>
    class Storage
    {
    public:
        template <typename... Args>
        ComponentType& addComponent(Entity entity, Args&&... args)
        {
            return m_storage.insert(entity, ComponentType{{}, std::forward<Args>(args)...});
        }

        ComponentType* getComponent(Entity entity)
        {
            return m_storage.get(entity);
        }

        const auto& getEntities() const
        {
            return m_storage.getEntities();
        }

    private:
        SparseSet<ComponentType> m_storage{};
    };

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
        ComponentType* getComponent(Entity entity)
        {
            auto& storage{getStorage<ComponentType>()};
            return storage.getComponent(entity);
        }

        template <ComponentConcept ComponentType>
        Storage<ComponentType>& getStorage()
        {
            static Storage<ComponentType> storage{};
            return storage;
        }

    private:
        World() = default;
        World(const World&) = delete;
        World(World&&) = delete;

        Entity m_nextEntity{1};
    };

    // Systems
#pragma region Systems
    static void circle_renderer_system()
    {
        for (auto& world{World::getInstance()}; const auto& entity : world.getStorage<Circle>().getEntities())
        {
            const auto* circle_component{world.getComponent<Circle>(entity)};
            auto* transformComponent{world.getComponent<Transform>(entity)};

            auto [x,y] = transformComponent->position;
            DrawCircle(x, y, circle_component->radius, circle_component->color);
        }
    }
#pragma endregion
}

#endif //ECS_ECS_H
