#pragma once

namespace Dog {

	struct AssetData {
		uint64_t uuid = 0;
		uint8_t type = 0;
		uint64_t size = 0;
		uint64_t offset = 0;
		std::vector<char> data;
	};

	class DogFilePacker {
	public:
		static void packageAssets(const std::string& directory, const std::string& outputFile);
		static void unpackAssets(const std::string& packedFile);

		static void loadImages();
		static void loadShaders();
		static std::string getSceneYAMLString(const std::string& filePath);

		static std::string MapAssetTypeToString(uint8_t type);

		static std::vector<AssetData> unpackedData;
		static bool s_HasBeenUnpacked;
	};

}
