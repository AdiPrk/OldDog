#pragma once

#include "Dog/Resources/resource.h"

namespace Dog {

    // General purpose shader object. Compiles from file, generates
    // compile/link-time error messages and hosts several utility 
    // functions for easy management.
    class Shader : public Resource
    {
    public:
        static const std::string GetTypeName() { return "Shader"; }

        bool load(const std::string& path) override;
        void save(const std::string& path) override {};
        
        // state
        unsigned int ID;
        static int CurrentID;
        // constructor
        Shader() : ID() {}
        ~Shader();
        // sets the current shader as active
        Shader& Use();
        // compiles the shader from given source code
        bool Compile(const char* vertexSource, const char* fragmentSource); // note: geometry source code is optional 
        void SetUniformsFromCode();
        // utility functions
        void SetFloat(const std::string& name, float value);
        void SetUnsigned(const std::string& name, unsigned int value);
        void SetInteger(const std::string& name, int value);
        void SetVector2f(const std::string& name, float x, float y);
        void SetVector2f(const std::string& name, const glm::vec2& value);
        void SetVector3f(const std::string& name, float x, float y, float z);
        void SetVector3f(const std::string& name, const glm::vec3& value);
        void SetVector4f(const std::string& name, float x, float y, float z, float w);
        void SetVector4f(const std::string& name, const glm::vec4& value);
        void SetMatrix4(const std::string& name, const glm::mat4& matrix);
        void SetUniformHandle(const std::string& name, GLuint64 handle);
        bool HasUniform(const std::string& name);

        // a global variable across all shaders
        static float iTime;

        // uniforms
        std::unordered_map<std::string, GLuint> Uniforms;

        // UBO for projection and view matrices, binding point 0
        static GLuint uboMatrices;
        static GLuint uboMatricesBindingPoint;
        static GLuint uboTime;
        static GLuint uboTimeBindingPoint;
        static void SetupUBO();
        static void SetProjectionUBO(const glm::mat4& projection);
        static void SetViewUBO(const glm::mat4& view);
        static void SetProjectionViewUBO(const glm::mat4& projectionView);
        static void SetViewAndProjectionView(const glm::mat4& view, const glm::mat4& projectionView);
        static void SetTimeUBO(float time);
        void BindUBO(const std::string& blockName, unsigned int bindingPoint);

        static void SetShader(std::shared_ptr<Shader> shader) { activeShader = shader->Use(); }
        static void SetShader(Shader& shader) { activeShader = shader.Use(); }
        static Shader& GetActiveShader() { return activeShader; }
    private:
        // checks if compilation or linking failed and if so, print the error logs
        bool checkCompileErrors(unsigned int object, std::string type);
        bool loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile);

        static Shader activeShader;
    };

}