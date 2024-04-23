#include <PCH/dogpch.h>

#ifndef DOG_SHIP

#include "assetsWindow.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "textEditorWindow.h"

namespace Dog {

	FileBrowser::FileBrowser()
		: baseDir(Assets::AssetsDir)
	{
		currentDir = baseDir;
	}

	bool SameDir(const std::string& d1, const std::string& d2) {
		// dirs may or may not have trailing backslash, so just check for string minus backslash
		if (d1.empty() && d2.empty()) return true;
		if (d1.empty() || d2.empty()) return false;
		std::string d1_ = d1;
		std::string d2_ = d2;
		if (d1_.back() == '\\' || d1_.back() == '/') d1_.pop_back();
		if (d2_.back() == '\\' || d2_.back() == '/') d2_.pop_back();

		return d1_ == d2_;
	}

	void UpdateAssetsWindow(FileBrowser& browser, TextEditorWrapper& textEditor) {
		ImGui::Begin("Assets");

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
		if (currentDirName == Assets::ImagesDir || currentDirName == Assets::ShadersDir) {
			ImGui::SliderFloat("Image Size", &imageSize, 16, 512);
		}

		ImGui::Columns(columnCount, 0, false);

		// padding between images:
		int i = 0;
		for (const auto& entry : std::filesystem::directory_iterator(browser.currentDir)) {
			const auto& path = entry.path();
						
			// Directories
			if (entry.is_directory()) {
				if (SameDir(path.filename().string(), Assets::EditorDir)) continue;

				if (ImGui::Button(path.filename().string().c_str())) {
					browser.currentDir = path;
				}
			}
			// Files
			else {
				if (currentDirName == "") {
					continue;
				}

				ImGui::Spacing();
				ImGui::Spacing();

				
				// Different behavior depending on the directory.
				if (SameDir(Assets::AssetsDir, currentDirName)) {
					// Nothing to be done, really. Just default I guess?
					// Most likely there's no files here and it never comes to this point.
					ImGui::Text(path.filename().string().c_str());
				}
				else if (SameDir(currentDirName, Assets::ImagesDir)) {
					// Get the texture from the path
					std::string fileName = path.filename().string();
					std::string filePath = path.string();

					// image name to use:
					// std::string imagePath = Assets::AssetsDir + currentDirName + "/" + fileName;

					// tex id
					unsigned texId = 0;
					std::shared_ptr<Texture2D> tex = Assets::Get<Texture2D>(fileName);
					if (tex) {
						texId = tex->ID;
					}
					else {
						texId = Assets::Get<Texture2D>("error.png")->ID;
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
				else if (SameDir(currentDirName, Assets::ShadersDir)) {
					// Get the texture from the path
					std::string fileName = path.filename().string();
					std::string filePath = path.string();
					std::string shaderAssetName = fileName;
					shaderAssetName = shaderAssetName.substr(0, shaderAssetName.find_last_of("."));

					// tex id
					unsigned texId = 0;
					std::shared_ptr<Texture2D> tex = Assets::Get<Texture2D>("glslIcon.png");
					if (tex) {
						texId = tex->ID;
					}
					else {
						texId = Assets::Get<Texture2D>("error.png")->ID;
					}

					std::string fullPath = path.string();

					bool clicked = ImGui::ImageButton(fullPath.c_str(), (void*)(intptr_t)texId, ImVec2(imageSize, imageSize));

					bool isImageHovered = ImGui::IsItemHovered();
					bool doubleClicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

					// get if item is double clicked
					if (isImageHovered && doubleClicked) {
						std::string shaderPath = Assets::ShadersPath + fileName;

						TextEditorWrapper::MyDocument shaderDoc(fileName, true, shaderPath);
						textEditor.CreateNewDocument(shaderDoc);
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ImGui::SetDragDropPayload("Shader", shaderAssetName.c_str(), shaderAssetName.size() + 1);
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
					ImGui::TextWrapped(shaderAssetName.c_str());
				}

				ImGui::NextColumn();
			}

		}

		ImGui::Columns(1);

		ImGui::End(); // Assets
	}

}

#endif // DOG_SHIP