#include <PCH/dogpch.h>
#include "texture2d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Dog {

    GLuint Texture2D::CurrentTextureID = 0;

    bool Texture2D::load(const std::string& file)
    {
        stbi_set_flip_vertically_on_load(true);

        // check if file exists
        std::ifstream infile(file);
        if (!infile.good())
        {
            std::cerr << "Error: File does not exist: " << file << std::endl;
            return false;
        }
        infile.close();

        // load image
        int width, height, nrChannels;
        unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

        // check if data is loaded successfully
        if (!data)
        {
            std::cerr << "Error: Failed to load image: " << file << std::endl;
            return false;
        }

        // Set the internal and image formats based on the number of channels
        if (nrChannels == 4)
        {
            Internal_Format = GL_RGBA;
            Image_Format = GL_RGBA;
        }
        else if (nrChannels == 3)
        {
            Internal_Format = GL_RGB;
            Image_Format = GL_RGB;
        }

        // Calculate the width and height of a single sprite
        unsigned columns = 1, rows = 1;

        SpriteWidth = width / columns;
        SpriteHeight = height / rows;
        Rows = rows;
        Columns = columns;
        Index = 0;
        IsSpriteSheet = columns != 1 || rows != 1;

        Generate(width, height, data, rows * columns);

        // and finally free image data
        stbi_image_free(data);

        return true;
    }

    bool Texture2D::loadFromData(const std::vector<char>& data)
    {
        // load image
		int width, height, nrChannels;
		unsigned char* imageData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data.data()), (int)data.size(), &width, &height, &nrChannels, 0);

		// check if data is loaded successfully
        if (!imageData)
        {
			std::cerr << "Error: Failed to load image from data." << std::endl;
			return false;
		}

		// Set the internal and image formats based on the number of channels
        if (nrChannels == 4)
        {
			Internal_Format = GL_RGBA;
			Image_Format = GL_RGBA;
		}
        else if (nrChannels == 3)
        {
			Internal_Format = GL_RGB;
			Image_Format = GL_RGB;
		}

		// Calculate the width and height of a single sprite
		unsigned columns = 1, rows = 1;

		SpriteWidth = width / columns;
		SpriteHeight = height / rows;
		Rows = rows;
		Columns = columns;
		Index = 0;
		IsSpriteSheet = columns != 1 || rows != 1;

		Generate(width, height, imageData, rows * columns);

		// and finally free image data
		stbi_image_free(imageData);

		return true;
    }

    Texture2D::Texture2D()
        : Width(0)
        , Height(0)
        , Rows(1)
        , Columns(1)
        , Index(0)
        , IsSpriteSheet(false)
        , Internal_Format(GL_RGB)
        , Image_Format(GL_RGB)
        , Wrap_S(GL_REPEAT)
        , Wrap_T(GL_REPEAT)
        , Filter_Min(GL_LINEAR)
        , Filter_Max(GL_LINEAR)
        , textureHandle()
        , SpriteWidth(0)
        , SpriteHeight(0)
        , NumSprites(1)
        , ID(0)
    {
        unsigned int id;
        glGenTextures(1, &id);
        this->ID = id;

        // log id
        DOG_INFO("Texture2D constructor called. ID: {0}", this->ID);
        DOG_INFO("Texture2D constructor called. ID: {0}", id);
    }

    Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &this->ID);

        DOG_INFO("Texture2D destructor called.");
	}

    void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data, unsigned int numSprites)
    {
        this->Width = width;
        this->Height = height;
        this->NumSprites = numSprites;

        // create Texture
        if (numSprites == 1) {
            glBindTexture(GL_TEXTURE_2D, this->ID);

            glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, Width, Height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
        }
        else {
            // glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
            // glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, width, height, 1);
            // glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, Internal_Format, GL_UNSIGNED_BYTE, data);

            glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);

            // rgba8 works, rgba doesn't, ?
            glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, SpriteWidth, SpriteHeight, NumSprites);

            glPixelStorei(GL_UNPACK_ROW_LENGTH, Width);

            for (int i = 0; i < static_cast<int>(NumSprites); ++i)
            {
                int xOffSet = i % Columns * SpriteWidth;
                int yOffSet = (i / Columns) * SpriteHeight;
                glPixelStorei(GL_UNPACK_SKIP_PIXELS, xOffSet);
                glPixelStorei(GL_UNPACK_SKIP_ROWS, yOffSet);

                // printf("Index: %i, xOffset: %i, yOffset: %i\n", i, xOffSet, yOffSet);

                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, SpriteWidth, SpriteHeight, 1, Internal_Format, GL_UNSIGNED_BYTE, data);
            }

            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // Generate a handle for the texture and make it resident
        textureHandle = glGetTextureHandleARB(this->ID);
        glMakeTextureHandleResidentARB(textureHandle);

        // Reset stuff
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    }

    void Texture2D::Bind() const
    {
        if (this->ID != CurrentTextureID) {
            glBindTexture(GL_TEXTURE_2D, this->ID);
            CurrentTextureID = this->ID;
        }
    }

}