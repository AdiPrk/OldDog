#pragma once

#include "Dog/core.h"

namespace Dog {

    class DOG_API EntityImpl; // Forward declaration
    class DOG_API Scene;

    class DOG_API Entity {
    public:
        Entity(Scene* scene);
        ~Entity();

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent();

    private:
        std::unique_ptr<EntityImpl> impl; // Pimpl idiom
    };

}

#include "instantiation.h"