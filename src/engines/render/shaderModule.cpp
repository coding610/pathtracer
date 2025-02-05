#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <regex>
#include <string>

#include <engines/render/shaderModule.hpp>
#include <debug.hpp>
#include <globals.hpp>



//////////////////////
////// Builders //////
//////////////////////
ShaderModule::ShaderModule() { }
ShaderModule::~ShaderModule() { }


//////////////////
////// Main //////
//////////////////
void ShaderModule::init(const ShaderCrate& crate) { }
void ShaderModule::useShader(const char* name) {
    glUseProgram(shaders.at(name));
}

void ShaderModule::setUniform(const char* shaderName, const char* uniformName, const float& value) {
    glUniform1f(glGetUniformLocation(shaders.at(shaderName), uniformName), value);
}

void ShaderModule::loadShader(const char* name, const char* vertPath, const char* fragPath) {
    GLuint vertexShader = compileShader(vertPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragPath, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    ////// Check for linking errors //////
    GLint success; glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512]; glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Fatal Error: App::RenderEngine::ShaderModule::loadShader::glCreateProgram().\n-------- INFO LOG - INFO LOG - INFO LOG - INFO LOG --------: \n\n\n\n\n").append(infoLog).append("\n\n\n\n\n"));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    shaders[name] = shaderProgram;
}

GLuint ShaderModule::compileShader(const char* path, GLenum shaderType) {
    ////// Load File Contents //////
    std::ifstream file(path); std::stringstream buffer; buffer << file.rdbuf();
    std::string shaderSource = buffer.str();

    ////// Process Custom Macros//////
    ShaderModule::processMacros(path, shaderSource);

    ////// Create Shader //////
    const char* src = shaderSource.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    ////// Check for compilation errors //////
    GLint success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512]; glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Fatal Error: App::RenderEngine::ShaderModule::compileShader::glCompileShader().\n-------- INFO LOG - INFO LOG - INFO LOG - INFO LOG --------: \n\n\n\n\n")
            .append(infoLog)
        );
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
void ShaderModule::processMacros(const char* path, std::string& shaderSource) {
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
        if (!file.is_open()) { throw std::runtime_error("Fatal Error: App::RenderEngine::ShaderModule::compileShader::processMacros. Failed to open included shader " + includePath.string()); }

        std::stringstream buffer; std::string firstLine;
        if (std::getline(file, firstLine)) { buffer << file.rdbuf(); }

        processedSource += "\n/* --- START INCLUDED FILE: " + includePath.string() + " --- */\n";
        processedSource += buffer.str();
        processedSource += "\n/* --- END INCLUDED FILE: " + includePath.string() + " --- */\n";

        lastPos = matchPos + match.length(0);
    }

    processedSource.append(shaderSource, lastPos, shaderSource.size() - lastPos);
    shaderSource = std::move(processedSource);

    if (DEBUG) {
        std::ofstream procFile(folderPath.string() + "/processed/" + std::filesystem::path(path).stem().string() + ".proc.glsl", std::ios::out);
        procFile << shaderSource; procFile.close();
    }
}
