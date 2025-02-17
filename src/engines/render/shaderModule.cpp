#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <iostream>

#include <engines/render/shaderModule.hpp>


//////////////////////
////// Builders //////
//////////////////////
ShaderModule::ShaderModule() { }
ShaderModule::~ShaderModule() { }


//////////////////
////// Main //////
//////////////////
void ShaderModule::useShader(const char* name) {
    glUseProgram(shaders.at(name));
}

void ShaderModule::setUniform(const char* shaderName, const char* uniformName, const float& value) {
    glUniform1f(glGetUniformLocation(shaders.at(shaderName), uniformName), value);
}

void ShaderModule::loadShader(const char* name, const char* vertPath, const char* fragPath) {
    spdlog::info("Compiling \t vertexShader \t [4.4.[{}].1]", name);
    GLuint vertexShader = compileShader(name, vertPath, GL_VERTEX_SHADER);
    spdlog::info("Compiling \t fragmentShader  [4.4.[{}].2]", name);
    GLuint fragmentShader = compileShader(name, fragPath, GL_FRAGMENT_SHADER);

    spdlog::info("Creating \t shaderProgram \t [4.4.[{}].3]", name);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    ////// Check for linking errors //////
    spdlog::info("Verifying \t shaderProgram \t [4.4.[{}].4]", name);
    GLint success; glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512]; glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        spdlog::critical("ShaderProgram initialization failed [4.4.[{}].4.E1]", name);
        std::cout << infoLog << std::endl; std::abort();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaders[name] = shaderProgram;
}

void ShaderModule::loadShader(const char* name, const char* compPath) {
    spdlog::info("Compiling \t compPath \t [4.4.[{}].1]", name);
    GLuint computeShader = compileShader(name, compPath, GL_COMPUTE_SHADER);

    spdlog::info("Creating \t shaderProgram \t [4.4.[{}].2]", name);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, computeShader);
    glLinkProgram(shaderProgram);

    ////// Check for linking errors //////
    spdlog::info("Verifying \t shaderProgram \t [4.4.[{}].3]", name);
    GLint success; glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512]; glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        spdlog::critical("ShaderProgram initialization failed [4.4.[{}].3.E1]", name);
        std::cout << infoLog << std::endl; std::abort();
    }

    glDeleteShader(computeShader);
    shaders[name] = shaderProgram;
}

GLuint ShaderModule::compileShader(const char* name, const char* path, GLenum shaderType) {
    ////// Load File Contents //////
    std::ifstream file(path); std::stringstream buffer; buffer << file.rdbuf();
    std::string shaderSource = buffer.str();

    ////// Process Custom Macros//////
    spdlog::info("Processing \t shaderSource \t [4.4.[{}].1.[{}].1]", name, std::filesystem::path(path).stem().string());
    ShaderModule::processMacros(name, path, shaderSource);

    ////// Create Shader //////
    const char* src = shaderSource.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    ////// Check for compilation errors //////
    GLint success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512]; glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        spdlog::info("Failed compiling {} shader [4.4.[{}].1.[{}].E1]", std::filesystem::path(path).stem().string(), name, std::filesystem::path(path).stem().string());
        std::cout << infoLog << std::endl; std::abort();
    }

    return shader;
}


/*
- Uses regex to find [[ path ]] occurenses in shader script.
- Replaces the occurenses with the file, treating it like
  a macro.
- Removes first line of #version xxx
- Creates a processed debug file to view when DEBUG global is enabled.
*/
void ShaderModule::processMacros(const char* name, const char* path, std::string& shaderSource) {
    std::filesystem::path folderPath = std::filesystem::path(path).parent_path();
    std::regex macroRegex(R"(\[\[\s*([\w\d_.-]+)\s*\]\])");

    std::string processedSource;
    size_t lastPos = 0;
    std::sregex_iterator it(shaderSource.begin(), shaderSource.end(), macroRegex);
    std::sregex_iterator end;

    for (; it != end; ++it) {
        std::smatch match = *it;
        size_t matchPos = match.position(0);

        processedSource.append(shaderSource, lastPos, matchPos - lastPos);
        std::filesystem::path includePath = folderPath / match[1].str();

        std::ifstream file(includePath);
        if (!file.is_open()) {
            spdlog::critical("Failed to open included shader [4.4.[{}].1.[{}].1.E1]", name, std::filesystem::path(path).stem().string()); std::abort();
        }

        std::stringstream buffer; std::string firstLine;
        if (std::getline(file, firstLine)) { buffer << file.rdbuf(); }

        processedSource += "\n/* --- START INCLUDED FILE: " + includePath.string() + " --- */\n";
        processedSource += buffer.str();
        processedSource += "\n/* --- END INCLUDED FILE: " + includePath.string() + " --- */\n";

        lastPos = matchPos + match.length(0);
    }

    processedSource.append(shaderSource, lastPos, shaderSource.size() - lastPos);
    shaderSource = std::move(processedSource);


    if (spdlog::get_level() <= spdlog::level::debug) {
        const std::string stem = std::filesystem::path(path).stem().string();
        const std::string procPath = folderPath.string() + "/processed/" + stem + ".proc.glsl";
        spdlog::info("Writing {} shader to [{}] [4.4.[{}].1.[{}].1.1]", stem, procPath, name, stem);

        std::ofstream procFile(procPath); procFile << shaderSource; procFile.close();
    }
}
