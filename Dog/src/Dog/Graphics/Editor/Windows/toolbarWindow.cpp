#include <PCH/dogpch.h>
#include "toolbarWindow.h"
#include "Dog/engine.h"
#include "Dog/Graphics/Texture/texture2d.h"

namespace Dog {

	bool isPlaying = false;

	void OnPlay();
	void OnStop();

	void UpdateToolbarWindow() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.6f, 0.5f));

		ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoResize);

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		// get tex id
		unsigned texId = 0;
		if (isPlaying) {
			auto tex = Resources::GetImage<Texture2D>("stopButton.png");
			if (tex) {
				texId = tex->ID;
			}
		}
		else {
			auto tex = Resources::GetImage<Texture2D>("playButton.png");
			if (tex) {
				texId = tex->ID;
			}
		}

		// draw the image centered in this window.
		float imageWidth = 60.0f;
		float imageHeight = 40.0f;

		if (ImGui::ImageButton((void*)(uintptr_t)texId, ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)))
		{
			if (isPlaying) {
				OnStop();
			}
			else {
				OnPlay();
			}
		}		

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End(); // toolbar
	}

	void OnPlay() {
		isPlaying = true;
		PUBLISH_EVENT(Event::PlayButtonPressed);
	}

	void OnStop() {
		isPlaying = false;
		PUBLISH_EVENT(Event::StopButtonPressed);
	}
}
