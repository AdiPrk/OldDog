#pragma once

namespace Dog {

	class FileBrowser;
	class TextEditorWrapper;

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

		void beginFrame(bool renderEditor);
		void endFrame();

		void SetEditorEnabled(bool enabled) { renderEditor = enabled; }
		bool GetEditorEnabled() const { return renderEditor; }

	private:
		std::unique_ptr<FileBrowser> fileBrowser;
		std::unique_ptr<TextEditorWrapper> textEditorWrapper;
		bool renderEditor = true;
		bool startedRenderingFrame = false;
	};

} // namespace Dog
