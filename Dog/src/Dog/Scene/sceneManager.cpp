#include <PCH/dogpch.h>
#include "sceneManager.h"
#include "scene.h"

namespace Dog {

	SceneManager SceneManager::m_Instance;
	Scene* SceneManager::m_ActiveScene = nullptr;
	Scene* SceneManager::m_NextScene = nullptr;
	bool SceneManager::m_IsRestarting = false;

	void SceneManager::SetNextScene(Scene* next)
	{
		if (!next) return;

		if (m_NextScene == next) {
			m_IsRestarting = true;
		}
		else {
			m_NextScene = next;
		}
	}

	void SceneManager::Init(Scene* startScene)
	{
		m_NextScene = startScene;
	}

	void SceneManager::Update(float dt)
	{
		// Check for a scene change.
		if (IsChangingScenes())
		{
			// Exit the current scene.
			Exit();

			if (m_IsRestarting) {
				m_IsRestarting = false;
			}
			else {
				m_ActiveScene = m_NextScene;
			}

			// Initialize the new scene.
			m_ActiveScene->InternalInit();
			m_ActiveScene->Init();
		}

		// Update the active scene.
		m_ActiveScene->InternalUpdate(dt);
		m_ActiveScene->Update(dt);
	}

	void SceneManager::Render(float dt)
	{
		m_ActiveScene->InternalRender(dt);
		m_ActiveScene->Render(dt);
	}

	void SceneManager::Exit(void)
	{
		if (m_ActiveScene) {
			m_ActiveScene->Exit();
			m_ActiveScene->InternalExit();
		}
	}

}