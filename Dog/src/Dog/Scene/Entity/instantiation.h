#include "common.h"

namespace Dog {

// Macro for explicit template instantiation
#define INSTANTIATE_COMPONENT(Type) \
    template DOG_API Type& Entity::AddComponent<Type>(); \
    template DOG_API Type& Entity::GetComponent<Type>(); \
    template DOG_API bool Entity::HasComponent<Type>();

    
// Explicitly instantiate all components
    INSTANTIATE_COMPONENT(TagComponent)

}
