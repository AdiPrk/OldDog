#include <PCH/dogpch.h>
#include "editor.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/engine.h"
#include "Dog/Resources/resources.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"
#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Graphics/Renderer/Camera/camera2d.h"
#include "Dog/Graphics/Renderer/Camera/orthoCamera.h"
#include "Dog/Graphics/Renderer/Camera/orthoCameraController.h"
#include "Dog/Input/input.h"

namespace Dog {
	Editor::Editor()
	{
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
		ImGui::ShowDemoWindow(); // Show demo window! :)

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		auto tex = Engine::GetResources()->Get<Texture2D>("DogAssets/Images/circle.png");

		Scene* currentScene = SceneManager::GetCurrentScene();
		if (currentScene) {
			std::shared_ptr<FrameBuffer> fbo = currentScene->GetFrameBuffer();
			if (!fbo) {
				DOG_WARN("No FrameBuffer found in the current scene.");
			}
			else {
				unsigned fboID = fbo->GetColorAttachmentID();
				static ImVec2 lastSceneWindowSize = { 0.0f, 0.0f };
				ImVec2 vpSize = ImGui::GetContentRegionAvail(); // viewport size

				bool viewportFocused = ImGui::IsWindowFocused();
				bool viewportHovered = ImGui::IsWindowHovered();

				// Lock key input if the viewport is not focused.
				Input::SetKeyInputLocked(!viewportFocused);

				// Lock mouse input if the viewport is not hovered.
				Input::SetMouseInputLocked(!viewportHovered);

				// get if winSize changed
				if (vpSize.x != lastSceneWindowSize.x || vpSize.y != lastSceneWindowSize.y) {

					PUBLISH_EVENT(Event::SceneResize, (int)vpSize.x, (int)vpSize.y);

					// fbo->OnResize({ (int)vpSize.x, (int)vpSize.y });
					// Engine::GetCamera()->OnResize(vpSize.x, vpSize.y);
					// Engine::GetCameraController()->OnResize(vpSize.x, vpSize.y);

					lastSceneWindowSize = vpSize;
				}

				ImGui::Image((void*)(uintptr_t)fboID, vpSize);
			}
		}
		else {
			DOG_ASSERT(false, "No scene found.");
		}

		ImGui::End(); // End of Scene Window
		ImGui::PopStyleVar(); // Window Padding
	}

	void Editor::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

} // namespace Dog