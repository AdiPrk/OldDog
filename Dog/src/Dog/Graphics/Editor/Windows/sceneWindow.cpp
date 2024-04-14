#include <PCH/dogpch.h>
#include "sceneWindow.h"

#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"
#include "Dog/Graphics/Renderer/Renderer2D/texture2d.h"
#include "Dog/engine.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

namespace Dog {

	void UpdateSceneWindow()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		Scene* currentScene = SceneManager::GetCurrentScene();
		if (currentScene) {
			const auto& fbo = currentScene->GetFrameBuffer();

			if (!fbo) {
				DOG_WARN("No FrameBuffer found in the current scene.");
			}
			else {
				unsigned fboID = fbo->GetColorAttachmentID();

				static ImVec2 lastSceneWindowSize = { 0.0f, 0.0f };
				ImVec2 vpSize = ImGui::GetContentRegionAvail(); // viewport size

				// Lock key input if the viewport is not focused.
				Input::SetKeyInputLocked(!ImGui::IsWindowFocused());

				// Lock mouse input if the viewport is not hovered.
				Input::SetMouseInputLocked(!ImGui::IsWindowHovered());

				// Update things if viewport size changed (eg camera, framebuffer)
				if (vpSize.x != lastSceneWindowSize.x || vpSize.y != lastSceneWindowSize.y) {

					PUBLISH_EVENT(Event::SceneResize, (int)vpSize.x, (int)vpSize.y);
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

}
