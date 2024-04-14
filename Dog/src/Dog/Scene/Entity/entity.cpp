#include <PCH/dogpch.h>

#include "entity.h"

#include "../scene.h"



namespace Dog {

    Entity::Entity(Scene* scene_)
    {
        scene = scene_;
        handle = scene->GetRegistry().create();
    }

    Entity::Entity(const Entity& other) 
    {
        scene = other.scene;
        handle = scene->GetRegistry().create();

        // Copy all components
        /*entt::registry& registry = scene->GetRegistry();
        entt::registry& otherRegistry = other.scene->GetRegistry();*/

    }

    Entity::~Entity() {
        // Shouldn't get destroyed here.
        // The scene should destroy it in the scene's destructor
        // Or when asked by the user

        // scene->GetRegistry().destroy(handle);
    }

}