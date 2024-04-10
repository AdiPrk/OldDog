#include <PCH/dogpch.h>
#include "renderer2d.h"
#include "texture2d.h"

namespace Dog {

	Renderer2D::~Renderer2D()
	{
	}

	void Renderer2D::initialize()
	{
        // configure VAO/VBO/EBO
        unsigned int VBO, EBO;
        float vertices[] = {
            // pos
            0.0f, 1.0f, 0.0f, 1.0f, // Top-left vertex
            0.0f, 0.0f, 0.0f, 0.0f, // Bottom-left vertex
            1.0f, 1.0f, 1.0f, 1.0f, // Top-right vertex
            1.0f, 0.0f, 1.0f, 0.0f  // Bottom-right vertex
        };

        unsigned int indices[] = {
            0, 1, 2, // First Triangle
            1, 3, 2  // Second Triangle
        };

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(this->quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::beginFrame()
	{
        glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer2D::endFrame()
	{

	}

    void Renderer2D::DrawSprite(const Texture2D& texture, const glm::mat4& transform, glm::vec4 color, glm::vec2 repetition, float depth) const
    {
        Shader::GetActiveShader().SetMatrix4("model", transform);
        //Renderer::GetActiveShader().SetFloat("depth", RandomFloat());

        // render textured quad
        Shader::GetActiveShader().SetVector4f("spriteColor", color);

        if (texture.IsSpriteSheet) {
            static unsigned index = texture.Index;
            if (texture.Index != index) {
                index = texture.Index;
                Shader::GetActiveShader().SetUnsigned("spriteIndex", index);
            }
        }

        if ((repetition.x == 0 || repetition.y == 0)) {
            Shader::GetActiveShader().SetVector2f("repetition", 1.0f, 1.0f);
        }
        else {
            glm::vec2 size = {
                glm::length(glm::vec3(transform[0])),
                glm::length(glm::vec3(transform[1]))
            };
            Shader::GetActiveShader().SetVector2f("repetition", size.x / repetition.x, size.y / repetition.y);
        }

        // Set the texture handle as a uniform
        Shader::GetActiveShader().SetUniformHandle("textureHandle", texture.textureHandle);

        glBindVertexArray(this->quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Renderer2D::DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, glm::vec2 repetition, float depth) const
    {
        // prepare transformations
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position + size * 0.5f, 0.0f));  // first translate
        model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // translate
        model = glm::scale(model, glm::vec3(size, 1.0f)); // scale by size
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));

        DrawSprite(texture, model, color, repetition, depth);
    }

}