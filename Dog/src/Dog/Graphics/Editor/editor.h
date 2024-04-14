#pragma once



namespace Dog {

	class Editor {
	public:
		Editor();
		~Editor();

		static Editor& Get() {
			static Editor instance;
			return instance;
		}

		Editor(const Editor&) = delete;
		Editor& operator=(const Editor&) = delete;

		void Init(GLFWwindow* window);
		void Exit();

		void beginFrame();
		void endFrame();
	};

} // namespace Dog
