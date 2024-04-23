#include <PCH/dogpch.h>

#ifndef DOG_SHIP

#include "editor.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"

#include "Windows/sceneWindow.h"
#include "Windows/consoleWindow.h"
#include "Windows/entitiesWindow.h"
#include "Windows/inspectorWindow.h"
#include "Windows/toolbarWindow.h"
#include "Windows/assetsWindow.h"
#include "Windows/textEditorWindow.h"
#include "Windows/noEditorWindow.h"

#include "Dog/Assets/Packer/assetPacker.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"
#include "Dog/Scene/Serializer/sceneSerializer.h"


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

		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if (!isActive) {
			return;
		}

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene")) {}
			if (ImGui::MenuItem("Save Scene")) 
			{
				Scene* currentScene = SceneManager::GetCurrentScene();
				if (currentScene != nullptr) {
					std::string sceneName = currentScene->GetName();

					SceneSerializer::Serialize(currentScene, "DogAssets/Scenes/" + sceneName + ".yaml");
				}
				else {
					DOG_WARN("No scene to save!");
				}
			}
			
			// open scene, which shows all scenes
			if (ImGui::BeginMenu("Open Scene")) {

				// get all files in the scene directory (std filesystem)
				std::vector<std::string> files;
				for (const auto& entry : std::filesystem::directory_iterator("DogAssets/Scenes")) {
					files.push_back(entry.path().string());
				}

				// display all files in the scene directory
				for (const auto& file : files) {

					// remove the directory path
					std::string fileName = file.substr(file.find_last_of("/\\") + 1);

					if (ImGui::MenuItem(fileName.c_str())) {

						Scene* currentScene = SceneManager::GetCurrentScene();

						if (currentScene != nullptr) {
							std::string sceneName = currentScene->GetName();

							SceneSerializer::Deserialize(currentScene, "DogAssets/Scenes/" + sceneName + ".yaml");
						}
						else {
							DOG_WARN("No scene to deserialize to????");
						}

					}
				}

				ImGui::EndMenu(); // Open Scene
			}


			if (ImGui::MenuItem("Exit")) {}
			if (ImGui::MenuItem("Create Asset Pack")) {
				DogFilePacker::packageAssets("DogAssets", "DogAssets/fetch.it");
			}
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
		UpdateAssetsWindow(*fileBrowser, *textEditorWrapper);
		UpdateTextEditorWindow(*textEditorWrapper);
	}

	void Editor::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Editor::UpdateVisibility(unsigned windowWidth, unsigned windowHeight)
	{
		ImGuiIO& io = ImGui::GetIO();

		static bool renderEditor = isActive;
		static bool keyHeld = false;
		bool firstGameFrame = false;
		if (io.KeyCtrl && io.KeyShift && io.KeysDown[ImGuiKey_J])
		{
			if (!keyHeld) {
				renderEditor = !renderEditor;
				isActive = renderEditor;
				keyHeld = true;
				firstGameFrame = true;

				if (!renderEditor) {
					// trigger glfw window resize callback manually
					PUBLISH_EVENT(Event::SceneResize, (int)windowWidth, (int)windowHeight);
					glViewport(0, 0, windowWidth, windowHeight);
					Input::SetKeyInputLocked(false);
					Input::SetMouseInputLocked(false);
				}
			}
		}
		else {
			keyHeld = false;
		}
	}

} // namespace Dog

#endif // DOG_SHIP
