#include <PCH/dogpch.h>

#ifndef DOG_SHIP

#include "noEditorWindow.h"
#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/engine.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

namespace Dog {
	
	void UpdateNoEditorWindow(bool firstFrame) {	
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("NoEditor##noEditor", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);

		// make window full screen
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

		// Make sure inputs are not locked
		Input::SetKeyInputLocked(false);
		Input::SetMouseInputLocked(false);

		Scene* currentScene = SceneManager::GetCurrentScene();
		if (currentScene) {
			const auto& fbo = currentScene->GetFrameBuffer();

			if (!fbo) {
				DOG_WARN("No FrameBuffer found in the current scene.");
			}
			else {
				unsigned fboID = fbo->GetColorAttachmentID(0);

				static ImVec2 lastSceneWindowSize = { 0.0f, 0.0f };
				ImVec2 vpSize = ImGui::GetContentRegionAvail(); // viewport size		

				// Update things if viewport size changed (eg camera, framebuffer)
				if (firstFrame || vpSize.x != lastSceneWindowSize.x || vpSize.y != lastSceneWindowSize.y) {
					firstFrame = false;
					PUBLISH_EVENT(Event::SceneResize, (int)vpSize.x, (int)vpSize.y);

					// log vp size and last window size
					DOG_INFO("Viewport Size: {0}, {1}", vpSize.x, vpSize.y);
					DOG_INFO("Last Window Size: {0}, {1}", lastSceneWindowSize.x, lastSceneWindowSize.y);

					lastSceneWindowSize = vpSize;
				}

				ImGui::Image((void*)(uintptr_t)fboID, vpSize, { 0, 1 }, { 1, 0 });
			}
		}
		else {
			DOG_ASSERT(false, "No scene found.");
		}

		ImGui::End(); // End of Scene Window
		ImGui::PopStyleVar(); // Window Padding
	}

}

#endif