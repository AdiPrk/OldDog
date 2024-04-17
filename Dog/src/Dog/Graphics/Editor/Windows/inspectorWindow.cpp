#include <PCH/dogpch.h>
#include "inspectorWindow.h"
#include "entitiesWindow.h"
#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"
#include "Dog/Scene/Entity/entity.h"
#include "Dog/Scene/Entity/components.h"
#include "Dog/Graphics/Renderer/Camera/orthoCamera.h"
#include "Dog/Graphics/Renderer/Camera/perspectiveCamera.h"

namespace Dog {

	void DisplayComponents(Entity entity);

	template <typename T>
	void DisplayAddComponent(Entity entity, const std::string& name);

	void UpdateInspectorWindow() {
		ImGui::Begin("Inspector");

		static Entity lastSelectedEntity;
		Entity selectedEntity = GetSelectedEntity();

		bool entityChanged = lastSelectedEntity != selectedEntity;

		if (entityChanged) {
			lastSelectedEntity = selectedEntity;
		}

		if (!selectedEntity) {
			ImGui::Text("No entity selected.");
			ImGui::End(); // Inspector
			return;
		}

		if (entityChanged) {
			// ImGui::Text("Switching Entities...");
			// ImGui::End(); // Inspector
			// return; // don't display for one frame.
		}

		ImGui::PushID(selectedEntity);

		// Display components
		DisplayComponents(selectedEntity);

		// Add components button
		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("AddComponentPopup");
		}

		if (ImGui::BeginPopup("AddComponentPopup")) {
			DisplayAddComponent<TagComponent>(selectedEntity, "Tag");
			DisplayAddComponent<TransformComponent>(selectedEntity, "Transform");
			DisplayAddComponent<SpriteComponent>(selectedEntity, "Sprite");
			DisplayAddComponent<ShaderComponent>(selectedEntity, "Shader");
			DisplayAddComponent<CameraComponent>(selectedEntity, "Camera");

			ImGui::EndPopup();
		}

		ImGui::PopID();

		ImGui::End(); // Inspector
	}

	template <typename T>
	void DisplayAddComponent(Entity entity, const std::string& name) {
		if (!entity.HasComponent<T>() && ImGui::MenuItem(name.c_str())) {
			entity.AddComponent<T>();
		}
	}

	void RenderTagComponent(TagComponent& tagComponent) {
		auto& tag = tagComponent.Tag;

		ImGui::InputText("Name##TagProp", &tag);
		
		/*bool imguiDrawCursor = false;
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("The entity's name.");
			imguiDrawCursor = true;
		}

		ImGui::GetIO().MouseDrawCursor = imguiDrawCursor;*/
	}

	void RenderTransformComponent(TransformComponent& transform) {
		ImGui::SetNextItemOpen(false, ImGuiCond_FirstUseEver);
		
		if (ImGui::CollapsingHeader("Transform##header")) {
			ImGui::DragFloat3("Position##TransformProp", &transform.Translation.x, 0.1f);
			ImGui::DragFloat3("Rotation##TransformProp", &transform.Rotation.x, 0.1f);
			ImGui::DragFloat3("Scale##TransformProp", &transform.Scale.x, 0.1f);
		}
	}

	void RenderSpriteComponent(SpriteComponent& sprite) {
		ImGui::SetNextItemOpen(false, ImGuiCond_FirstUseEver);

		if (ImGui::CollapsingHeader("Sprite##header")) {
			// Don't make the input text hide the cursor.
			
			ImGui::InputText("Texture##SpriteProp", &sprite.texturePath);
			/*bool imguiDrawCursor = false;
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Drag and drop a texture here.");
				imguiDrawCursor = true;
			}

			ImGui::GetIO().MouseDrawCursor = ImGui::GetIO().MouseDrawCursor || imguiDrawCursor;*/

			if (ImGui::BeginDragDropTarget()) {
				// Accept data that has the identifier "DRAG_INT"
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture2D")) {
					// get string from payload data and data size
					std::string path = std::string((char*)payload->Data, payload->DataSize - 1); // -1 to remove null terminator
					sprite.texturePath = path;
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::ColorEdit4("Color##SpriteProp", &sprite.Color.r);
		}
	}

	void RenderShaderComponent(ShaderComponent& shader) {
		ImGui::SetNextItemOpen(false, ImGuiCond_FirstUseEver);

		if (ImGui::CollapsingHeader("Shader##header")) {
			std::string shaderPath = shader.shaderPath;
			size_t pos = shaderPath.find_last_of("/");
			if (pos != std::string::npos) {
				shaderPath = shaderPath.substr(pos + 1);
			}

			ImGui::InputText("Shader##ShaderProp", &shaderPath);

			if (ImGui::BeginDragDropTarget()) {
				// Accept data that has the identifier "DRAG_INT"
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Shader")) {
					// get string from payload data and data size
					std::string path = std::string((char*)payload->Data, payload->DataSize - 1); // -1 to remove null terminator
					shaderPath = path;
				}

				ImGui::EndDragDropTarget();
			}

			shader.shaderPath = shaderPath;
		}
	}

	void RenderCameraComponent(CameraComponent& camera) {
		ImGui::SetNextItemOpen(false, ImGuiCond_FirstUseEver);

		if (ImGui::CollapsingHeader("Camera##header")) {
			if (camera.Projection == CameraComponent::CameraType::Orthographic) {
				ImGui::Text("Orthographic Camera");

				//OrthographicCamera& orthoCamera = *camera.orthoCamera.get();

				ImGui::DragFloat("Size##OrthoCameraProp", &camera.OrthographicSize, 0.1f);
				ImGui::DragFloat("Near##OrthoCameraProp", &camera.OrthographicNear, 0.1f);
				ImGui::DragFloat("Far##OrthoCameraProp", &camera.OrthographicFar, 0.1f);
			}
			else if (camera.Projection == CameraComponent::CameraType::Perspective) {
				ImGui::Text("Perspective Camera");

				//PerspectiveCamera& perspectiveCamera = *camera.perspCamera.get();

				ImGui::DragFloat("FOV##PerspCameraProp", &camera.PerspectiveFOV, 0.1f);
				ImGui::DragFloat("Near##PerspCameraProp", &camera.PerspectiveNear, 0.1f);
				ImGui::DragFloat("Far##PerspCameraProp", &camera.PerspectiveFar, 0.1f);
			}
		}
	}

	void DisplayComponents(Entity entity) {
		// get all components from the entity
		entt::registry& registry = entity.GetScene()->GetRegistry();

		if (entity.HasComponent<TagComponent>())
			RenderTagComponent(entity.GetComponent<TagComponent>());
		
		if (entity.HasComponent<TransformComponent>())
			RenderTransformComponent(entity.GetComponent<TransformComponent>());

		if (entity.HasComponent<SpriteComponent>())
			RenderSpriteComponent(entity.GetComponent<SpriteComponent>());

		if (entity.HasComponent<ShaderComponent>())
			RenderShaderComponent(entity.GetComponent<ShaderComponent>());

		if (entity.HasComponent<CameraComponent>())
			RenderCameraComponent(entity.GetComponent<CameraComponent>());
	}

}
