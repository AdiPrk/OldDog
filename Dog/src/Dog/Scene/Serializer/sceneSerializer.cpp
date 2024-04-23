#include <PCH/dogpch.h>
#include "sceneSerializer.h"
#include "conversions.h"
#include "../scene.h"
#include "../Entity/entity.h"
#include "../Entity/components.h"
#include "Dog/Assets/Packer/assetPacker.h"

namespace Dog {

	void SceneSerializer::Serialize(Scene* scene, const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->GetName();

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		
		entt::registry& registry = scene->GetRegistry();

		registry.view<TagComponent>().each
		([&](const auto& entityID, const TagComponent& transform)
		{
			Entity entity(scene, entityID);
			SerializeEntity(out, &entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::Deserialize(Scene* scene, const std::string& filepath)
	{
#ifndef DOG_SHIP
		YAML::Node data = YAML::LoadFile(filepath);
#else
		const std::string& sceneDataStr = DogFilePacker::getSceneYAMLString(filepath);
		YAML::Node data = YAML::Load(sceneDataStr);
#endif

		// check if file loaded
		if (!data)
		{
			DOG_ERROR("SceneSerializer::Deserialize: Scene {} not found", filepath);
			return;
		}

		if (!data["Scene"])
		{
			DOG_ERROR("SceneSerializer::Deserialize: Scene file does not contain a Scene tag");
			return;
		}

		scene->ClearEntities();

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];

		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name = entity["Entity"].as<std::string>();
				DOG_TRACE("Deserializing entity: {0}", name);

				Entity deserializedEntity = scene->CreateEmptyEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					glm::vec3 translation = transformComponent["Translation"].as<glm::vec3>();
					glm::vec3 rotation = transformComponent["Rotation"].as<glm::vec3>();
					glm::vec3 scale = transformComponent["Scale"].as<glm::vec3>();
					deserializedEntity.AddComponent<TransformComponent>(translation, rotation, scale);
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					glm::vec4 color = spriteComponent["Color"].as<glm::vec4>();
					std::string texturePath = spriteComponent["Texture"].as<std::string>();
					deserializedEntity.AddComponent<SpriteComponent>(color, texturePath);
				}
			}
		}
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		if (!entity->HasComponent<TagComponent>()) return;

		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity->GetComponent<TagComponent>().Tag;

		if (entity->HasComponent<TransformComponent>())
		{
			auto& tc = entity->GetComponent<TransformComponent>();

			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}

		// Camera component needs a lot of work in general before it's ready for serialization
		/*if (entity->HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "CameraType" << YAML::Value << entity->GetComponent<CameraComponent>().Projection;
			out << YAML::EndMap;
		}*/

		if (entity->HasComponent<SpriteComponent>())
		{
			auto& spriteComponent = entity->GetComponent<SpriteComponent>();

			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.Color;
			out << YAML::Key << "Texture" << YAML::Value << spriteComponent.texturePath;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}


	// Conversions for YAML serialization


}

