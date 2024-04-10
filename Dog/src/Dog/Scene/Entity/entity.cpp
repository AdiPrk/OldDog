#include <PCH/dogpch.h>
#include "entt/entt.hpp"

#include "entity.h"
#include "common.h"

#include "../scene.h"
#include "../sceneData.h"

namespace Dog {
        
    class DOG_API EntityImpl {
    public:
        EntityImpl(Scene* s)
        {
            scene = s;
            handle = s->GetSceneData()->registry.create();
        }

        Scene* scene;
        entt::entity handle;
    };

    Entity::Entity(Scene* scene)
        : impl(std::make_unique<EntityImpl>(scene))
    {
    }

    Entity::~Entity() = default;

    template<typename T, typename... Args>
    T& Entity::AddComponent(Args&&... args) {
        return impl->scene->GetSceneData()->registry.emplace<T>(impl->handle, std::forward<Args>(args)...);
    }

    template<typename T>
    T& Entity::GetComponent() {
        return impl->scene->GetSceneData()->registry.get<T>(impl->handle);
    }

    template<typename T>
    bool Entity::HasComponent() {
        return impl->scene->GetSceneData()->registry.all_of<T>(impl->handle);
    }

}