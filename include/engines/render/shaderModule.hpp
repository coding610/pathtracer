/*
-- ShaderModule of RenderEngine --

- Binds shaders during setup.
- Ensuring the appropriate layout during runtime.
*/

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <unordered_map>


struct ShaderCrate { };

class ShaderModule {
public:
    ShaderModule();
    ~ShaderModule();

    void init(const ShaderCrate& crate);
    void loadShader(const char* name, const char* vertPath, const char* fragPath);
    void useShader(const char* name);
    void setUniform(const char* shaderName, const char* uniformName, const float& value);

    void processMacros(const char* path, std::string& shaderSource);

private:
    GLuint compileShader(const char* path, GLenum shaderType);

    std::unordered_map<const char*, GLuint> shaders;
};
