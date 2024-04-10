#pragma once

#include "entt/entt.hpp"
#include "scene.h"

namespace Dog {

	class Entity;

	class Scene::SceneData
	{
	public:
		entt::registry registry;
	};

}
