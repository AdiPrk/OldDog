#include <PCH/dogpch.h>
#include "sceneWindow.h"

#include "Dog/Graphics/Framebuffer/framebuffer.h"
#include "Dog/Input/input.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/engine.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

namespace Dog {

	ImVec2 sceneImagePosition = { 0.0f, 0.0f };

	ImVec2 GetRelativeSceneImagePosition() {
		return sceneImagePosition;
	}

	void UpdateSceneWindow()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		// 2d/3d button toggle with checkbox
		static bool is3D = false;

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 30.0f - ImGui::CalcTextSize("3D").x, 25.0f));
		ImGui::Checkbox("3D", &is3D);

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

				// Lock key input if the viewport is not focused.
				Input::SetKeyInputLocked(!ImGui::IsWindowFocused());

				// Lock mouse input if the viewport is not hovered.
				Input::SetMouseInputLocked(!ImGui::IsWindowHovered());

				// Update things if viewport size changed (eg camera, framebuffer)
				if (vpSize.x != lastSceneWindowSize.x || vpSize.y != lastSceneWindowSize.y) {

					PUBLISH_EVENT(Event::SceneResize, (int)vpSize.x, (int)vpSize.y);
					lastSceneWindowSize = vpSize;
				}

				// ImVec2 content_region_origin = ImGui::GetWindowContentRegionMin();
				float image_x = ImGui::GetCursorPosX();
				float image_y = ImGui::GetCursorPosY();
				sceneImagePosition = { image_x, image_y };

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
