#include <PCH/dogpch.h>
#include "resourcesWindow.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "textEditorWindow.h"

namespace Dog {

	FileBrowser::FileBrowser()
		: baseDir(Resources::AssetsDir)
	{
		currentDir = baseDir;
	}

	bool SameDir(const std::string& d1, const std::string& d2) {
		// dirs may or may not have trailing backslash, so just check for string minus backslash
		std::string d1_ = d1;
		std::string d2_ = d2;
		if (d1_.back() == '\\' || d1_.back() == '/') d1_.pop_back();
		if (d2_.back() == '\\' || d2_.back() == '/') d2_.pop_back();

		return d1_ == d2_;
	}

	void UpdateResourcesWindow(FileBrowser& browser, TextEditorWrapper& textEditor) {
		ImGui::Begin("Resources");

		if (browser.currentDir != browser.baseDir) {
			if (ImGui::Button("Back")) {
				browser.currentDir = browser.baseDir;
			}
		}

		static float padding = 16.0f;
		static float imageSize = 100.0f;
		float cellSize = imageSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		std::string currentDirName = browser.currentDir.filename().string();
		if (currentDirName == Resources::ImagesDir || currentDirName == Resources::ShadersDir) {
			ImGui::SliderFloat("Image Size", &imageSize, 16, 512);
		}

		ImGui::Columns(columnCount, 0, false);

		// padding between images:
		int i = 0;
		for (const auto& entry : std::filesystem::directory_iterator(browser.currentDir)) {
			const auto& path = entry.path();
						
			// Directories
			if (entry.is_directory()) {
				if (SameDir(path.filename().string(), Resources::EditorDir)) continue;

				if (ImGui::Button(path.filename().string().c_str())) {
					browser.currentDir = path;
				}
			}
			// Files
			else {
				ImGui::Spacing();
				ImGui::Spacing();
				
				// Different behavior depending on the directory.
				if (SameDir(Resources::AssetsDir, currentDirName)) {
					// Nothing to be done, really. Just default I guess?
					// Most likely there's no files here and it never comes to this point.
					ImGui::Text(path.filename().string().c_str());
				}
				else if (SameDir(currentDirName, Resources::ImagesDir)) {
					// Get the texture from the path
					std::string fileName = path.filename().string();
					std::string filePath = path.string();

					// image name to use:
					std::string imagePath = Resources::AssetsDir + currentDirName + "/" + fileName;

					// tex id
					unsigned texId = 0;
					std::shared_ptr<Texture2D> tex = Resources::Get<Texture2D>(imagePath);
					if (tex) {
						texId = tex->ID;
					}
					else {
						texId = Resources::GetImage<Texture2D>("error.png")->ID;
					}

					bool clicked = ImGui::ImageButton((void*)(intptr_t)texId, ImVec2(imageSize, imageSize));

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("Texture2D", fileName.c_str(), fileName.size() + 1);
						ImGui::Image((void*)(intptr_t)texId, ImVec2(imageSize, imageSize));
						ImGui::Text(fileName.c_str());
						ImGui::EndDragDropSource();
					}

					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text(fileName.c_str());
						ImGui::EndTooltip();
					}
					
					// for multi-line centered text
					ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str(), NULL, true, imageSize);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (imageSize - textSize.x) * 0.5f);
					ImGui::TextWrapped(fileName.c_str());
				}
				else if (SameDir(currentDirName, Resources::ShadersDir)) {
					// Get the texture from the path
					std::string fileName = path.filename().string();
					std::string filePath = path.string();

					// only display if it is a .vert file
					// and then truncate the vert part.
					if (fileName.find(".vert") == std::string::npos) {
						continue;
					}

					fileName = fileName.substr(0, fileName.find(".vert"));

					// tex id
					unsigned texId = 0;
					std::shared_ptr<Texture2D> tex = Resources::GetImage<Texture2D>("glslIcon.png");
					if (tex) {
						texId = tex->ID;
					}
					else {
						texId = Resources::GetImage<Texture2D>("error.png")->ID;
					}

					std::string fullPath = path.string();

					bool clicked = ImGui::ImageButton(fullPath.c_str(), (void*)(intptr_t)texId, ImVec2(imageSize, imageSize));

					bool isImageHovered = ImGui::IsItemHovered();
					bool doubleClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

					// get if item is double clicked
					if (isImageHovered && doubleClicked) {
						std::string vertName = fileName + ".vert";
						std::string fragName = fileName + ".frag";
						std::string vertPath = Resources::ShadersPath + vertName;
						std::string fragPath = Resources::ShadersPath + fragName;

						TextEditorWrapper::MyDocument vertDoc(vertName, true, vertPath);
						textEditor.CreateNewDocument(vertDoc);

						TextEditorWrapper::MyDocument fragDoc(fragName, true, fragPath);
						textEditor.CreateNewDocument(fragDoc);

						// printf("Adding shader: %s\n", fileName.c_str());
						// printf("\tvert path: %s\n", vertPath.c_str());
						// printf("\tfrag path: %s\n", fragPath.c_str());
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("Shader", fileName.c_str(), fileName.size() + 1);
						ImGui::Image((void*)(intptr_t)texId, ImVec2(imageSize, imageSize));
						ImGui::Text(fileName.c_str());
						ImGui::EndDragDropSource();
					}

					if (ImGui::IsItemHovered()) {
						ImGui::BeginTooltip();
						ImGui::Text(fileName.c_str());
						ImGui::EndTooltip();
					}

					// for multi-line centered text
					ImVec2 textSize = ImGui::CalcTextSize(fileName.c_str(), NULL, true, imageSize);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (imageSize - textSize.x) * 0.5f);
					ImGui::TextWrapped(fileName.c_str());
				}

				ImGui::NextColumn();
			}

		}

		ImGui::Columns(1);

		ImGui::End(); // Resources
	}

}
