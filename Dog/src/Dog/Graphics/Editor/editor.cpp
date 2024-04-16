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

namespace Dog {
	Editor::Editor()
	{
		fileBrowser = std::make_shared<FileBrowser>();
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
		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		

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
		UpdateResourcesWindow(*fileBrowser);
	}

	void Editor::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

} // namespace Dog