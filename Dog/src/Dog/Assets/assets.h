#pragma once

namespace Dog {
	
	class AssetFactory;
	class Texture2D;
	class Shader;
	struct AssetData;
	class Asset;

#define INSTANTIATE_GET_EXT(T) \
	extern template std::shared_ptr<T> Assets::Get<T>(const std::string& filePath); \
	extern template std::shared_ptr<T> Assets::Get<T>(const UUID& uuid)

	class Assets {
	public:
		static void Init();
		static void Shutdown();

		static std::shared_ptr<Asset> Load(const std::string& type, const std::string& filePath);
		static std::shared_ptr<Asset> LoadFromData(const AssetData& data);

		// templated Load
		template <typename T>
		static std::shared_ptr<T> Load(const std::string& filePath) {
			// static assert if T doesn't derive from asset
			static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

			std::string type = T::GetTypeName();
			auto asset = Load(type, filePath);
			return std::dynamic_pointer_cast<T>(asset);
		}

		// Load from the packed file
		template <typename T>
		static std::shared_ptr<T> LoadFromData(const AssetData& data) {
			// static assert if T doesn't derive from asset
			static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

			std::string type = T::GetTypeName();
			auto asset = LoadFromData(data);
			return std::dynamic_pointer_cast<T>(asset);
		}

		// Get asset by filePath
		// Requires full relative path
		template <typename T>
		static std::shared_ptr<T> Get(const std::string& filePath);

		template <typename T>
		static std::shared_ptr<T> Get(const UUID& uuid);
		
		static void Unload(const std::string& type, const std::string& filePath);
		static void UnloadAll();

		inline static const std::string AssetsDir = "DogAssets/";

		inline static const std::string EditorDir = "Editor/";
		inline static const std::string ShadersDir = "Shaders/";
		inline static const std::string ImagesDir = "Images/";
		inline static const std::string ScenesDir = "Scenes/";

		inline static const std::string EditorPath = "DogAssets/Editor/";
		inline static const std::string ShadersPath = "DogAssets/Shaders/";
		inline static const std::string ImagesPath = "DogAssets/Images/";
		inline static const std::string ScenesPath = "DogAssets/Scenes/";

	private:
		static std::unordered_map<UUID, std::shared_ptr<Asset>> assets;
		static AssetFactory* factory;
		static std::mutex assetsMutex;

		friend class Engine;
		enum class FileAction {
			Create,
			Delete,
			Modify
		};
		static std::vector<std::pair<FileAction, std::string>> imageFileActions;
		static std::vector<std::pair<FileAction, std::string>> shaderFileActions;

		static void UpdateAssets(const float& deltaTime);
		static void OnImageFileCreate(const Event::ImageFileCreated& event);
		static void OnImageFileDelete(const Event::ImageFileDeleted& event);
		static void OnImageFileModify(const Event::ImageFileModified& event);
		static void OnShaderFileCreate(const Event::ShaderFileCreated& event);
		static void OnShaderFileDelete(const Event::ShaderFileDeleted& event);
		static void OnShaderFileModify(const Event::ShaderFileModified& event);

		std::mutex fileActionsMutex;
	};

	INSTANTIATE_GET_EXT(Texture2D);
	INSTANTIATE_GET_EXT(Shader);
}
