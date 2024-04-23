#include <PCH/dogpch.h>
#include "assetPacker.h"

#include "Dog/engine.h"
#include "Dog/Graphics/Window/window.h"
#include "Dog/Logger/logger.h"
#include "Dog/Graphics/Renderer/Renderer2D/renderer2d.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Graphics/Renderer/Renderer3D/DeferredRenderer.h"
#include "Dog/Input/input.h"

#include "Dog/Scene/sceneManager.h"
#include "Dog/Scene/scene.h"

#include "Dog/Graphics/Editor/editor.h"

#include "Dog/Assets/FileWatcher/fileWatcher.h"

#include "Dog/Graphics/Window/frameRate.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"

#include "Dog/Assets/assets.h"
#include "Dog/Assets/asset.h"

namespace fs = std::filesystem;

namespace Dog {

    /**
     * Format:
     * 
     * First three chars: DOG
     * 
     * Then in sets of four
     * - Hash of the file path (uint64_t)
     * - Asset type (uint8_t)
     * - Size of the asset (uint64_t)
     * - Offset to the asset data (uint64_t)
     * 
     * The rest of the file:
     * - Asset data
     */

    bool DogFilePacker::s_HasBeenUnpacked = false;
    std::vector<AssetData> DogFilePacker::unpackedData;

    enum class AssetType {
        Unknown = 0,
        Texture = 1,
        Shader = 2,
        Scene = 3,
    };

    class FilePacker {
    public:
        FilePacker(const std::string& directory, const std::string& outputFile)
            : directory(directory)
            , outputFile(outputFile)
            , output(outputFile, std::ios::binary)
        {
            if (!output.is_open()) {
                std::cerr << "Failed to open output file for writing." << std::endl;
                return;
            }

            pack();
        }

        void Write(const char* data, std::streamsize _Count) {
            output.write(data, _Count);
            totalSize += _Count;
        }

        void pack() {
            Write("DOG", 3);  // DOG header

            processDirectory(directory); // Process all directories

            WriteAssetData(); // Write asset data to file
        }

        void processDirectory(const fs::path& dir) {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (entry.is_directory()) {
                    processDirectory(entry.path());  // Recursively process subdirectories
                }
                else if (entry.is_regular_file()) {
                    std::ifstream file(entry.path(), std::ios::binary);
                    if (!file.is_open()) {
                        std::cerr << "Failed to open " << entry.path() << std::endl;
                        continue;
                    }

                    // Process file and add to assetData
                    AssetData asset;
                    asset.type = getAssetType(entry.path());

                    // Don't add these to the packed file
                    if (asset.type == static_cast<uint8_t>(AssetType::Unknown)) {
						continue;
					}

                    std::string filePath = entry.path().string();
                    // in filepath convert \\ to /
                    std::replace(filePath.begin(), filePath.end(), '\\', '/');

                    // if type is shader, remove the extension
                    if (asset.type == static_cast<uint8_t>(AssetType::Shader)) {
						filePath = filePath.substr(0, filePath.find_last_of('.'));
					}

                    asset.uuid = UUID(filePath);
                    asset.size = fs::file_size(entry);
                    asset.data.resize(asset.size);
                    file.read(asset.data.data(), asset.size);
                    asset.offset = 0; // this gets updated later

                    assetData.push_back(asset);

                    ++assetCount;
                }
            }
        }

        void WriteAssetData() {
            // Write asset data to file //

            // First write the asset count to the file
            Write(reinterpret_cast<const char*>(&assetCount), sizeof(uint64_t));

            // calculate size of the whole asset info block
            uint64_t assetHeaderSize = assetCount * (sizeof(uint64_t) + sizeof(uint8_t) + sizeof(uint64_t) + sizeof(uint64_t));

            uint64_t assetDataOffset = totalSize + assetHeaderSize;

            // Write all the asset info first
            for (auto& asset : assetData) {
                /*
                - Hash of the file path (uint64_t)
                - Asset type (uint8_t)
                - Size of the asset (uint64_t)
                - Offset to the asset data (uint64_t)
                */
                Write(reinterpret_cast<const char*>(&asset.uuid), sizeof(asset.uuid));
                Write(reinterpret_cast<const char*>(&asset.type), sizeof(asset.type));
                Write(reinterpret_cast<const char*>(&asset.size), sizeof(asset.size));
                Write(reinterpret_cast<const char*>(&assetDataOffset), sizeof(assetDataOffset));

                assetDataOffset += asset.size;
            }

            // Now write the asset data
            for (auto& asset : assetData) {
                Write(asset.data.data(), asset.size);
            }
        }

    private:
        std::ofstream output;
        std::string directory;
        std::string outputFile;
        uint64_t totalOffset = 0;
        uint64_t totalSize = 0;
        uint64_t assetCount = 0;

        std::vector<AssetData> assetData;


        uint8_t getAssetType(const fs::path& path) {
            return static_cast<uint8_t>(deduceAssetType(path));
        }

        // Determine the asset type based on file extension
        AssetType deduceAssetType(const fs::path& path) {
            auto ext = path.extension().string();
            if (ext == ".png" || ext == ".jpg") return AssetType::Texture;
            else if (ext == ".glsl") return AssetType::Shader;
			else if (ext == ".yaml") return AssetType::Scene;

            return AssetType::Unknown;
        }
    };

    class FileUnpacker {
    public:
        FileUnpacker(const std::string& packedFile)
            : input(packedFile, std::ios::binary)
        {
            if (!input.is_open()) {
				std::cerr << "Failed to open packed file for reading." << std::endl;
				return;
			}

			unpack();

            DogFilePacker::unpackedData = assetData;
		}

        void unpack() {
            // First, read the header for DOG
            char header[3];
            input.read(header, 3);
            if (header[0] != 'D' || header[1] != 'O' || header[2] != 'G') {
				std::cerr << "Invalid packed file." << std::endl;
				return;
			}

            // Read the asset count
            uint64_t assetCount;
            input.read(reinterpret_cast<char*>(&assetCount), sizeof(uint64_t));

            // Read all the asset data
            assetData.reserve(assetCount);

            for (uint64_t i = 0; i < assetCount; ++i) {
				AssetData asset;
				input.read(reinterpret_cast<char*>(&asset.uuid), sizeof(asset.uuid));
				input.read(reinterpret_cast<char*>(&asset.type), sizeof(asset.type));
				input.read(reinterpret_cast<char*>(&asset.size), sizeof(asset.size));
				input.read(reinterpret_cast<char*>(&asset.offset), sizeof(asset.offset));

				assetData.push_back(asset);
			}

            // Now read the asset data
            for (auto& asset : assetData) {
                input.seekg(asset.offset);
                asset.data.resize(asset.size);
                input.read(asset.data.data(), asset.size);
            }

            // log the data to console
#ifndef DOG_SHIP
            printf("Unpacked:\n");
            for (auto& asset : assetData) {
				std::cout << "Asset: " << asset.uuid << " Type: " << static_cast<int>(asset.type) << " Size: " << asset.size << std::endl;
			}
#endif
        }

    private:
        std::ifstream input;

        std::vector<AssetData> assetData;
    };

    // Main function to package assets
    void DogFilePacker::packageAssets(const std::string& directory, const std::string& outputFile) {
        FilePacker packer(directory, outputFile); // they get packed in the constructor.
    }

    void DogFilePacker::unpackAssets(const std::string& packedFile) {
        FileUnpacker unpacker(packedFile);
        s_HasBeenUnpacked = true;
    }

    void DogFilePacker::loadImages()
    {
        for (auto& asset : unpackedData) {
            if (asset.type == static_cast<uint8_t>(AssetType::Texture)) {
                Assets::LoadFromData<Texture2D>(asset);
			}
		}
    }

    void DogFilePacker::loadShaders()
    {
        for (auto& asset : unpackedData) {
            if (asset.type == static_cast<uint8_t>(AssetType::Shader)) {
				Assets::LoadFromData<Shader>(asset);
            }
        }
    }

    std::string DogFilePacker::getSceneYAMLString(const std::string& filePath)
    {
        UUID uuid = UUID(filePath);

        for (auto& asset : unpackedData) {
            if (asset.uuid == uuid && asset.type == static_cast<uint8_t>(AssetType::Scene)) {
				return std::string(asset.data.begin(), asset.data.end());
			}
		}
        
		return "";
    }

    std::string DogFilePacker::MapAssetTypeToString(uint8_t type)
    {
        switch (type) {
            case static_cast<uint8_t>(AssetType::Texture): return Texture2D::GetTypeName();
            case static_cast<uint8_t>(AssetType::Shader): return Shader::GetTypeName();
            default: return "Unknown";
        }
    }

} // namespace Dog
