/*
-- ShaderModule of RenderEngine --

- Binds shaders during setup.
- Ensuring the appropriate layout during runtime.
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <string>
#include <unordered_map>


class ShaderModule {
public:
    ShaderModule();
    ~ShaderModule();

    void loadShader(const char* name, const char* vertPath, const char* fragPath);
    void loadShader(const char* name, const char* compPath);
    void useShader(const char* name);
    void setUniform(const char* shaderName, const char* uniformName, const float& value);

private:
    GLuint compileShader(const char* name, const char* path, GLenum shaderType);
    void processMacros(const char* name, const char* path, std::string& shaderSource);

    std::unordered_map<const char*, GLuint> shaders;
};
