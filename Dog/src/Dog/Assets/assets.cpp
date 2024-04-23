#include <PCH/dogpch.h>
#include "assets.h"
#include "assetFactory.h"
#include "asset.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Assets/Packer/assetPacker.h"

// Registration and instantiation macros
#define REGISTER_TYPE(type) factory->registerType<type>(type::GetTypeName())

#define INSTANTIATE_GET_DEF(T) \
	template std::shared_ptr<T> Assets::Get<T>(const std::string& filePath); \
	template std::shared_ptr<T> Assets::Get<T>(const UUID& uuid)

/**
 * -------------- When Registering a New Asset -------------.
 * 
 * Add the following line to the Assets::Init() function:
 * REGISTER_TYPE(YourAssetType);
 * 
 * Right below the Assets::Init() function, add the following line:
 * INSTANTIATE_GET_DEF(YourAssetType);
 * 
 * Also add an INTANTIATE_GET_EXT in assets.h next to the others. (end of the file)
 * 
 * Inside the Get function, add the asset's directory based on the type.
 * 
 */

namespace Dog {

	std::unordered_map<UUID, std::shared_ptr<Asset>> Assets::assets;
	AssetFactory* Assets::factory = nullptr;
	std::mutex Assets::assetsMutex;
	std::vector<std::pair<Assets::FileAction, std::string>> Assets::imageFileActions;
	std::vector<std::pair<Assets::FileAction, std::string>> Assets::shaderFileActions;

	void Assets::Init() {
		factory = new AssetFactory();

		// Registering the types with the asset factory
		REGISTER_TYPE(Shader);
		REGISTER_TYPE(Texture2D);

#ifdef DOG_SHIP
		DogFilePacker::unpackAssets("fetch");
		DogFilePacker::loadImages();
		DogFilePacker::loadShaders();
#endif
	}

	// Explicit template instantiations for the Get function
	INSTANTIATE_GET_DEF(Texture2D);
	INSTANTIATE_GET_DEF(Shader);

	void Assets::Shutdown() {
		UnloadAll();
		delete factory;
	}

	std::shared_ptr<Asset> Assets::Load(const std::string& type, const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);

		DOG_INFO("Loading asset: {}", "'" + type + "' from " + filePath);

		// first check if it already exists
		UUID assetID = UUID(filePath);
		auto it = assets.find(assetID);
		if (it != assets.end()) {
			DOG_WARN("Warning: Loading an already loaded asset: {}", filePath);
			DOG_WARN("Returning the previously loaded asset.");
			return it->second;
		}

		auto asset = factory->create(type);
		if (asset) {
			if (asset->load(filePath)) {
				assets[assetID] = asset;
			}
			else {
				DOG_ERROR("Failed to load asset: {}", filePath);
				DOG_INFO("*Sad Woof*");
				return nullptr;
			}
		}
		else {
			// also include file and line number that the log came from.
			DOG_INFO("*Sad Woof*");
			DOG_ASSERT(false, "Failed to load asset: {}", filePath);
		}
		return asset;
	}

	std::shared_ptr<Asset> Assets::LoadFromData(const AssetData& data)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);

		DOG_INFO("Loading asset from binary: {}", data.type);

		// first check if it already exists
		UUID assetID = data.uuid;
		auto it = assets.find(assetID);
		if (it != assets.end()) {
			DOG_WARN("Warning: Loading an already loaded asset: {}", data.uuid);
			DOG_WARN("Returning the previously loaded asset.");
			return it->second;
		}

		std::string assetType = DogFilePacker::MapAssetTypeToString(data.type);
		if (assetType == "Unknown") {
			DOG_ERROR("Failed to load asset from binary; Unkown type: {}, ID: {}", data.type, data.uuid);
			DOG_INFO("*Sad Woof*");
			return nullptr;
		}

		auto asset = factory->create(assetType);
		if (asset) {
			if (asset->loadFromData(data.data)) {
				assets[assetID] = asset;
			}
			else {
				DOG_ERROR("Failed to load asset from binary: {}; Type: {}", data.uuid, assetType);
				DOG_INFO("*Sad Woof*");
				return nullptr;
			}
		}
		else {
			// Also include file and line number that the log came from.
			// Factory did not have a create method for this asset.
			// Asset was likely not registered.
			DOG_INFO("*Sad Woof*");
			DOG_ASSERT(false, "Failed to load asset from binary: {}; Type: {}", data.uuid, assetType);
		}
		return asset;
	}

	template <typename T>
	static std::shared_ptr<T> Assets::Get(const std::string& filePath) {
		// static assert if T doesn't derive from asset
		static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

		std::string fileDir = "";
		if (std::is_same<T, Texture2D>::value) fileDir = ImagesPath;
		else if (std::is_same<T, Shader>::value) fileDir = ShadersPath;

		auto it = assets.find(UUID(fileDir + filePath));
		if (it != assets.end()) {
			auto asset = it->second;
			return std::dynamic_pointer_cast<T>(asset);
		}

		DOG_ERROR("Asset {0} not found in Assets", filePath);

		return nullptr;
	}

	template <typename T>
	std::shared_ptr<T> Assets::Get(const UUID& uuid) {
		auto it = assets.find(uuid);
		if (it != assets.end()) {
			return std::dynamic_pointer_cast<T>(it->second);
		}

		DOG_ERROR("Asset {0} not found in Assets", uuid);

		return nullptr;
	}

	void Assets::Unload(const std::string& type, const std::string& filePath)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);

		DOG_INFO("Unloading asset: {}", "'" + type + "' from " + filePath);

		auto it = assets.find(UUID(filePath));
		if (it != assets.end()) {
			assets.erase(it);
		}
		else {
			DOG_WARN("Warning: Unloading a non-loaded asset: {}", filePath);
		}
	}

	void Assets::UnloadAll()
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		assets.clear();
	}

	void Assets::UpdateAssets(const float& deltaTime) {
		Shader::iTime += deltaTime;
		Shader::SetTimeUBO(Shader::iTime);

		for (const auto& imageFileChange : imageFileActions) {
			switch (imageFileChange.first) {
			case FileAction::Create:
				Load("Texture2D", imageFileChange.second);
				break;
			case FileAction::Delete:
				Unload("Texture2D", imageFileChange.second);
				break;
			case FileAction::Modify:
				Unload("Texture2D", imageFileChange.second);
				Load("Texture2D", imageFileChange.second);
				break;
			default:
				break;
			}
		}

		imageFileActions.clear();

		for (const auto& shaderFileChange : shaderFileActions) {
			std::string fp = shaderFileChange.second;

			// if fp has an extension, remove it
			if (fp.find('.') != std::string::npos) {
				fp = fp.substr(0, fp.find('.'));
			}

			switch (shaderFileChange.first) {
			case FileAction::Create:
				Load("Shader", fp);
				break;
			case FileAction::Delete:
				Unload("Shader", fp);
				break;
			case FileAction::Modify:
				Unload("Shader", fp);
				Load("Shader", fp);
				break;
			default:
				break;
			}
		}

		shaderFileActions.clear();
	}

	void Assets::OnImageFileCreate(const Event::ImageFileCreated& event)
	{
		// lock mutex
		std::lock_guard<std::mutex> lock(assetsMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Create, event.path));
	}

	void Assets::OnImageFileDelete(const Event::ImageFileDeleted& event)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Delete, event.path));
	}

	void Assets::OnImageFileModify(const Event::ImageFileModified& event)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		imageFileActions.push_back(std::make_pair(FileAction::Modify, event.path));
	}

	void Assets::OnShaderFileCreate(const Event::ShaderFileCreated& event)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Create, event.path));
	}

	void Assets::OnShaderFileDelete(const Event::ShaderFileDeleted& event)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Delete, event.path));
	}

	void Assets::OnShaderFileModify(const Event::ShaderFileModified& event)
	{
		std::lock_guard<std::mutex> lock(assetsMutex);
		shaderFileActions.push_back(std::make_pair(FileAction::Modify, event.path));
	}

}
