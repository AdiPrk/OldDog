#include <PCH/dogpch.h>
#include "entitiesWindow.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"
#include "Dog/Scene/Entity/entity.h"
#include "Dog/Scene/Entity/components.h"

namespace Dog {

	Entity selectedEntity;

	Entity GetSelectedEntity() {
		return selectedEntity;
	}

	void UpdateEntitiesWindow() {
		ImGui::Begin("Entities");

		Scene* currentScene = SceneManager::GetCurrentScene();
		if (!currentScene) {
			ImGui::Text("No scene loaded");
			ImGui::End();
			return;
		}

		// Get registry
		entt::registry& registry = currentScene->GetRegistry();

		// Draw the Add in the menu bar of this window
		if (ImGui::BeginPopupContextWindow()) {
			
			// make a tree view like
			// 2d objects
			// - sprites
			// - - square
			// - - circle

			// start code here

			if (ImGui::BeginMenu("2D Objects")) {
				
				if (ImGui::BeginMenu("Sprites")) {

					if (ImGui::MenuItem("Square")) {
						Entity entity = currentScene->CreateEntity("Square");
						selectedEntity = entity;
					}
					if (ImGui::MenuItem("Circle")) {
						Entity entity = currentScene->CreateEntity("Circle");
						entity.GetComponent<SpriteComponent>().texturePath = "circle.png";
						selectedEntity = entity;
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		// Iterate through all entities with a tag, and without a parent

		auto view = registry.view<TagComponent>(entt::exclude<Parent>);
		for (auto& entityHandle : view) {
			Entity entity = { currentScene, entityHandle };
			
			TagComponent& tag = entity.GetComponent<TagComponent>();

			ImGuiTreeNodeFlags leafFlags = ImGuiTreeNodeFlags_Leaf;
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

			if (selectedEntity == entityHandle) {
				leafFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			bool opened = false;
			if (!entity.HasComponent<Children>()) {
				opened = ImGui::TreeNodeEx((void*)(uint64_t)entityHandle, leafFlags, tag.Tag.c_str());
			}
			else {
				opened = ImGui::TreeNodeEx((void*)(uint64_t)entityHandle, nodeFlags, tag.Tag.c_str());
			}

			if (ImGui::IsItemClicked()) {
				// Select entity
				DOG_INFO("Entity {0} Selected!", tag.Tag.c_str());
				selectedEntity = entity;
			}

			// end treenodex
			if (opened) {
				ImGui::TreePop();
			}
		}

		
		ImGui::End(); // End of Entities
	}

}
