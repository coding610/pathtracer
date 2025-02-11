/*
-- BufferModule of RenderEngine --

- Creates and initializes the SSBO and UBOs during setup
- Updates GPU buffers during runtime
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cstddef>
#include <unordered_map>


struct BufferCrate { };

class BufferModule {
public:
    BufferModule();
    ~BufferModule();
    void init(const BufferCrate& crate);

    void createBuffer(const char* name, GLenum type, size_t size, GLuint bindingPoint);
    void updateBuffer(const char* name, const void* data, size_t size);

private:
    struct Buffer {
        GLuint id;
        GLenum type;
        GLuint bindingPoint;
    };

    std::unordered_map<const char*, Buffer> buffers;
};
