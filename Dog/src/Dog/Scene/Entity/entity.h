#pragma once

#include "../scene.h"

namespace Dog {

    class Entity {
    public:
        Entity(Scene* scene);
        Entity(const Entity& other);
        ~Entity();

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return scene->GetRegistry().emplace<T>(handle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent() {
            return scene->GetRegistry().get<T>(handle);
        }

        template<typename T>
        bool HasComponent() {
            return scene->GetRegistry().all_of<T>(handle);
        }

    private:
        Scene* scene;
        entt::entity handle;
    };

}