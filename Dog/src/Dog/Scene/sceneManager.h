#pragma once

namespace Dog {

	class Scene;
	class Entity;

	class SceneManager
	{
	public:
		static SceneManager GetInstance() { return m_Instance; }
		static Scene* GetCurrentScene() { return m_ActiveScene; }
		static bool IsRunning() { return (m_ActiveScene || m_NextScene); }
		static void RestartCurrentScene() { m_IsRestarting = true; }
		static void SetNextScene(Scene* next);

		static void Init(Scene* startScene);
		static void SwapScenes();
		static void Update(float dt);
		static void Render(float dt, bool renderEditor);
		static void Exit();

	private:
		static SceneManager m_Instance;
		static Scene* m_ActiveScene;
		static Scene* m_NextScene;
		static bool m_IsRestarting;

		static bool IsChangingScenes() { return (m_IsRestarting || (m_ActiveScene != m_NextScene)); }
	};
}