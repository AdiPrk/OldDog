#include <PCH/dogpch.h>
#include "scene.h"
#include "Dog/Logger/logger.h"

#include "Entity/entity.h"
#include "Entity/common.h"
#include "sceneData.h"

#define DOG_SCENE_LOGGING 0

namespace Dog {

//#define REGISTER_COMPONENT(ComponentType) \
//    sceneData->componentAdders[Component::ComponentType] = [this](Entity* entity) { \
//        sceneData->registry.emplace<ComponentType##Component>(entity->m_EntityHandle); \
//    };

	Scene::Scene(const char* name)
	{
		strcpy_s(sceneName, sizeof(sceneName), name);
		sceneData = std::make_unique<SceneData>();

		//REGISTER_COMPONENT(Tag)
	}

	Scene::~Scene()
	{
	}

	void Scene::AddEntity(Entity* entity)
	{
		Entity newEnt(this);
		//entity->AddComponent(Component::Tag);

		newEnt.AddComponent<TagComponent>();
	}

	//void Scene::AddComponent(Entity* entity, const Component& component)
	//{
	//	auto it = sceneData->componentAdders.find(component);
	//	if (it != sceneData->componentAdders.end())
	//	{
	//		// Call the add respective component function.
	//		it->second(entity);
	//	}
	//	else
	//	{
	//		// Component not found!
	//	}
	//}

	void Scene::InternalInit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} initialized.", sceneName);
#endif
	}

	void Scene::InternalUpdate(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} updated.", sceneName);
#endif
	}

	void Scene::InternalRender(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} rendered.", sceneName);
#endif
	}

	void Scene::InternalExit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} exited.", sceneName);
#endif
	}

}
