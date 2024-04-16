#include <PCH/dogpch.h>
#include "shader.h"
#include <fstream>
#include <sstream>
#include "Dog/Logger/logger.h"

namespace Dog {

    float Shader::iTime = 0.0f;
    GLuint Shader::uboMatrices = 0;
    GLuint Shader::uboTime = 0;
    GLuint Shader::uboResolution = 0;
    GLuint Shader::uboMatricesBindingPoint = 0;
    GLuint Shader::uboTimeBindingPoint = 1;
    GLuint Shader::uboResolutionBindingPoint = 2;

    int Shader::CurrentID = 0;

    Shader Shader::activeShader;

    bool Shader::load(const std::string& path)
    {
        std::string vShaderFile = path + ".vert";
        std::string fShaderFile = path + ".frag";

        // check if those files exist
        std::ifstream vFile(vShaderFile);
        std::ifstream fFile(fShaderFile);
        if (!vFile.good() || !fFile.good()) {
            if (!vFile.good()) {
                DOG_CRITICAL("Shader files not found: {0}", vShaderFile);
            }
            if (!fFile.good()) {
                DOG_CRITICAL("Shader files not found: {0}", fShaderFile);
            }

            return false;
		}

        if (!loadShaderFromFile(vShaderFile, fShaderFile)) {
            return false;
        }

        // shader.SetUniformsFromCode(); // - an optimization which is really not worth completing for now.

        BindUBO("Matricies", Shader::uboMatricesBindingPoint);
        BindUBO("Time", Shader::uboTimeBindingPoint);
        BindUBO("Resolution", Shader::uboResolutionBindingPoint);

        return true;
    }

    bool Shader::loadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        try
        {
            // open files
            std::ifstream vertexShaderFile(vShaderFile);
            std::ifstream fragmentShaderFile(fShaderFile);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vertexShaderFile.rdbuf();
            fShaderStream << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
            return false;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. now create shader object from source code
        if (!Compile(vShaderCode, fShaderCode)) {
            return false;
        }

        return true;
    }

    Shader::~Shader()
    {
        glDeleteProgram(ID);
    }

    Shader& Shader::Use()
    {
        if (this->ID != CurrentID) {
            glUseProgram(this->ID);
            CurrentID = this->ID;
        }

        return *this;
    }

    bool Shader::Compile(const char* vertexSource, const char* fragmentSource)
    {
        unsigned int sVertex, sFragment;
        bool retVal = true;

        // vertex Shader 
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        if (!checkCompileErrors(sVertex, "VERTEX")) retVal = false;

        // fragment Shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        if (!checkCompileErrors(sFragment, "FRAGMENT")) retVal = false;

        // shader program
        this->ID = glCreateProgram();
        glAttachShader(this->ID, sVertex);
        glAttachShader(this->ID, sFragment);
        glLinkProgram(this->ID);
        if (!checkCompileErrors(this->ID, "PROGRAM")) retVal = false;

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);

        return retVal;
    }

    void Shader::SetUniformsFromCode()
    {
        GLint numUniforms = 0;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);

        GLchar uniformName[256]; // Adjust size as needed
        for (GLint i = 0; i < numUniforms; ++i) {
            GLint arraySize = 0;
            GLenum type = 0;
            GLsizei actualLength = 0;
            glGetActiveUniform(ID, i, sizeof(uniformName), &actualLength, &arraySize, &type, uniformName);

            GLint location = glGetUniformLocation(ID, uniformName);
            if (location != -1) { // -1 indicates that the uniform is not active
                std::string name(uniformName, actualLength); // Construct a string with the actual name length
                Uniforms[name] = location;
            }
        }
    }

    void Shader::SetFloat(const std::string& name, float value)
    {
        this->Use();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::SetUnsigned(const std::string& name, unsigned int value)
    {
        this->Use();
        glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::SetInteger(const std::string& name, int value)
    {
        this->Use();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void Shader::SetVector2f(const std::string& name, float x, float y)
    {
        this->Use();
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    void Shader::SetVector2f(const std::string& name, const glm::vec2& value)
    {
        this->Use();
        glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
    }
    void Shader::SetVector3f(const std::string& name, float x, float y, float z)
    {
        this->Use();
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    void Shader::SetVector3f(const std::string& name, const glm::vec3& value)
    {
        this->Use();
        glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
    }
    void Shader::SetVector4f(const std::string& name, float x, float y, float z, float w)
    {
        this->Use();
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    void Shader::SetVector4f(const std::string& name, const glm::vec4& value)
    {
        this->Use();
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
    }
    void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix)
    {
        this->Use();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(matrix));
    }

    void Shader::SetUniformHandle(const std::string& name, GLuint64 handle) {
        glUniformHandleui64ARB(glGetUniformLocation(ID, name.c_str()), handle);
    }

    bool Shader::HasUniform(const std::string& name)
    {
        return Uniforms.find(name) != Uniforms.end();
    }

    bool Shader::checkCompileErrors(unsigned int object, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;

                DOG_CRITICAL("Shader Compile-time error: Type: {0}", type);
                DOG_CRITICAL("{0}", infoLog);
                return false;
            }
        }
        else
        {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                    << infoLog << "\n -- --------------------------------------------------- -- "
                    << std::endl;

                DOG_CRITICAL("Shader Link-time error: Type: {0}", type);
                DOG_CRITICAL("{0}", infoLog);
                return false;
            }
        }

        return true;
    }

    void Shader::SetupUBO()
    {
        glGenBuffers(1, &uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 3 * sizeof(glm::mat4));

        glGenBuffers(1, &uboTime);
        glBindBuffer(GL_UNIFORM_BUFFER, uboTime);
        glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboTime, 0, sizeof(float));

        glGenBuffers(1, &uboResolution);
        glBindBuffer(GL_UNIFORM_BUFFER, uboResolution);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboResolution, 0, sizeof(glm::vec2));
    }

    void Shader::SetProjectionUBO(const glm::mat4& projection)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    }

    void Shader::SetViewUBO(const glm::mat4& view)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    }

    void Shader::SetProjectionViewUBO(const glm::mat4& projectionView)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionView));
    }

    void Shader::SetViewAndProjectionView(const glm::mat4& view, const glm::mat4& projectionView)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionView));
    }

    void Shader::SetTimeUBO(float time) {
        glBindBuffer(GL_UNIFORM_BUFFER, uboTime);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &time);
    }

    void Shader::SetResolutionUBO(const glm::vec2& resolution)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, uboResolution);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), glm::value_ptr(resolution));
    }

    void Shader::BindUBO(const std::string& blockName, unsigned int bindingPoint) {
        unsigned int blockIndex = glGetUniformBlockIndex(ID, blockName.c_str());
        if (blockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(ID, blockIndex, bindingPoint);
        }
    }

}