#include <PCH/dogpch.h>
#include "editor.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Resources/resources.h"
#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"

#include "Windows/sceneWindow.h"
#include "Windows/consoleWindow.h"
#include "Windows/entitiesWindow.h"
#include "Windows/inspectorWindow.h"
#include "Windows/toolbarWindow.h"
#include "Windows/resourcesWindow.h"
#include "Windows/textEditorWindow.h"
#include "Windows/noEditorWindow.h"

namespace Dog {
	Editor::Editor()
	{
		fileBrowser = std::make_unique<FileBrowser>();
		textEditorWrapper = std::make_unique<TextEditorWrapper>();
	}
	
	Editor::~Editor()
	{
	}

	void Editor::Init(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		
		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);  // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();
	}

	void Editor::Exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Editor::beginFrame()
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool keyHeld = false;
		bool firstGameFrame = false;
		if (io.KeyCtrl && io.KeyShift && io.KeysDown[ImGuiKey_J])
		{
			if (!keyHeld) {
				renderEditor = !renderEditor;
				keyHeld = true;
				firstGameFrame = true;
			}
		}
		else {
			keyHeld = false;
		}


		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!renderEditor) {
			UpdateNoEditorWindow(firstGameFrame);
			return;
		}

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene")) {}
			if (ImGui::MenuItem("Open Scene")) {}
			if (ImGui::MenuItem("Save Scene")) {}
			if (ImGui::MenuItem("Exit")) {}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar(); // File

		ImGui::ShowDemoWindow(); // Show demo window! :)

		// Update and render all the different windows
		UpdateSceneWindow();
		UpdateConsoleWindow();
		UpdateEntitiesWindow();
		UpdateInspectorWindow();
		UpdateToolbarWindow();
		UpdateResourcesWindow(*fileBrowser, *textEditorWrapper);
		UpdateTextEditorWindow(*textEditorWrapper);
	}

	void Editor::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

} // namespace Dog