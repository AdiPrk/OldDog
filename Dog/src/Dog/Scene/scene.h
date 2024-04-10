#pragma once

#include "Dog/core.h"
#include "Entity/entity.h"
#include "sceneManager.h"

namespace Dog {

	class DOG_API Scene {
	public:
		Scene(const char* name);
		~Scene();

		virtual void Init() {}
		virtual void Update(float dt) {}
		virtual void Render(float dt) {}
		virtual void Exit() {}

		const char* GetName() const { return sceneName; }

		class Entity CreateEntity();

	private:

		char sceneName[128];
		
		class SceneData;
		std::unique_ptr<SceneData> sceneData;
		
		friend EntityImpl;
		friend Entity;
		SceneData* GetSceneData() { return sceneData.get(); }

		friend SceneManager;
		void InternalInit();
		void InternalUpdate(float dt);
		void InternalRender(float dt);
		void InternalExit();
	};

}
