/*
-- utils.hpp --

- Standalone
- Inline header utils file

*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <engines/camera/cameraEngine.hpp>


namespace utils {

inline void setProjections(
    const CameraCrate& crate,
    const glm::vec2& dimensions,
    glm::mat4x4& projection,
    glm::mat4x4& view,
    glm::mat4x4& viewProjection,
    glm::mat4x4& inverseViewProjection
) {
    projection = glm::perspective(glm::radians(crate.fov), dimensions.x / dimensions.y, 0.1f, 100.0f);
    view = glm::lookAt(crate.position, crate.target, glm::vec3(0, 1, 0));
    viewProjection = projection * view;
    inverseViewProjection = glm::inverse(viewProjection);
}

inline void setFullscreenQuad(GLuint& VAO, GLuint& VBO, GLuint& EBO) {
    float vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,  1.0f, -1.0f,  1.0f };
    GLuint indices[] = { 0, 1, 2, 2, 3, 0 };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


}; // namespace utils
