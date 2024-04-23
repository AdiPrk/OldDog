#include <PCH/dogpch.h>
#include "renderer2d.h"
#include "Dog/Graphics/Texture/texture2d.h"
#include "Dog/Graphics/Renderer/Shaders/shader.h"

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
            -0.5f, 0.5f, 0.0f, 1.0f, // Top-left vertex
            -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left vertex
            0.5f, 0.5f, 1.0f, 1.0f, // Top-right vertex
            0.5f, -0.5f, 1.0f, 0.0f  // Bottom-right vertex
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

    void Renderer2D::clearFrame()
    {
        glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer2D::beginFrame()
    {
        clearFrame();
    }

    void Renderer2D::endFrame()
    {

    }

    void Renderer2D::DrawSprite(std::weak_ptr<Texture2D> textureP, std::weak_ptr<Shader> shaderP, const glm::mat4& transform, glm::vec4 color, glm::vec2 repetition, float depth)
    {
        std::shared_ptr<Texture2D> texture = textureP.lock();
        std::shared_ptr<Shader> shader = shaderP.lock();
        if (shader->ID != activeShaderID) {
            activeShaderID = shader->ID;
            Shader::SetShader(shader);
        }

        Shader& activeShader = Shader::GetActiveShader();

        activeShader.SetMatrix4("model", transform);

        // render textured quad
        activeShader.SetVector4f("spriteColor", color);

        if (texture->IsSpriteSheet) {
            static unsigned index = texture->Index;
            if (texture->Index != index) {
                index = texture->Index;
                activeShader.SetUnsigned("spriteIndex", index);
            }
        }

        if ((repetition.x == 0 || repetition.y == 0)) {
            activeShader.SetVector2f("repetition", 1.0f, 1.0f);
        }
        else {
            glm::vec2 size = {
                glm::length(glm::vec3(transform[0])),
                glm::length(glm::vec3(transform[1]))
            };
            activeShader.SetVector2f("repetition", size.x / repetition.x, size.y / repetition.y);
        }

        // Set the texture handle as a uniform
        activeShader.SetUniformHandle("textureHandle", texture->textureHandle);

        glBindVertexArray(this->quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

}