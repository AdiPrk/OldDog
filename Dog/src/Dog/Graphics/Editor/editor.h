#pragma once

namespace Dog {

	class FileBrowser;

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

	private:
		std::shared_ptr<FileBrowser> fileBrowser;
	};

} // namespace Dog
