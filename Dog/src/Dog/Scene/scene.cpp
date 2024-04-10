#include <PCH/dogpch.h>
#include "scene.h"
#include "Dog/Logger/logger.h"

#include "Entity/entity.h"
#include "Entity/components.h"
#include "sceneData.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"

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

	Entity Scene::CreateEntity()
	{
		Entity newEnt(this);

		TagComponent& tg = newEnt.AddComponent<TagComponent>();
		TransformComponent& tr = newEnt.AddComponent<TransformComponent>();
		SpriteComponent& sc = newEnt.AddComponent<SpriteComponent>();

		return newEnt;
	}

	void Scene::InternalInit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} init.", sceneName);
#endif
	}

	void Scene::InternalUpdate(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} Update.", sceneName);
#endif
	}

	void Scene::InternalRender(float dt)
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} Render.", sceneName);
#endif

		std::shared_ptr<Renderer2D>& renderer2D = Engine::Get().GetRenderer2D();

		renderer2D->beginFrame();

		sceneData->registry.view<TransformComponent, SpriteComponent>().each
		([renderer2D](const auto& entity, const TransformComponent& transform, const SpriteComponent& sprite) {
			renderer2D->DrawSprite(sprite.Texture, transform.GetTransform(), sprite.Color);
		});

		renderer2D->endFrame();
	}

	void Scene::InternalExit()
	{
#if DOG_SCENE_LOGGING
		DOG_INFO("Scene {0} exit.", sceneName);
#endif
	}

}
