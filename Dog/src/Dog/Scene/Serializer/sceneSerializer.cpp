#include <PCH/dogpch.h>
#include "sceneSerializer.h"
#include "conversions.h"
#include "../scene.h"
#include "../Entity/entity.h"
#include "../Entity/components.h"

namespace Dog {

	void SceneSerializer::Serialize(Scene* scene, const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

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
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "Untitled";

		if (entity->HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << entity->GetComponent<TagComponent>().Tag;
			out << YAML::EndMap;
		}

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
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << entity->GetComponent<SpriteComponent>().Color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}


	// Conversions for YAML serialization


}

