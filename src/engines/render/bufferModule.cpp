#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <spdlog/spdlog.h>

#include <engines/render/bufferModule.hpp>


///////////////////////
/////// Builders //////
///////////////////////
BufferModule::BufferModule() { }
BufferModule::~BufferModule() {
    for (const auto& [name, buffer] : buffers) { glDeleteBuffers(1, &buffer.id); }
    buffers.clear();
}


//////////////////
////// Main //////
//////////////////
void BufferModule::createBuffer(const char* name, GLenum type, size_t size, GLuint bindingPoint) {
    spdlog::info("Creating \t buffer \t [4.4.[{}].1", name);
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    glBufferData(type, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(type, bindingPoint, buffer);

    buffers[name] = { .id=buffer, .type=type, .bindingPoint=bindingPoint };
}

void BufferModule::updateBuffer(const char* name, const void* data, size_t size) {
    const Buffer& buffer = buffers[name];
    glBindBuffer(buffer.type, buffer.id);
    glBufferSubData(buffer.type, 0, size, data);
}
