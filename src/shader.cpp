#include "shader.h"
#include <exception>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

/**********************************************************************************/
/**********************************************************************************/

ShaderProgram& ShaderProgram::instance() {
    static ShaderProgram inst;
    return inst;
}

/**********************************************************************************/
/**********************************************************************************/

ShaderProgram::~ShaderProgram() {
    for (const auto& kv : this->vertexShaders) {
        unsigned int shader = kv.second;
        glDeleteShader(shader);
    }
    for (const auto& kv : this->fragmentShaders) {
        unsigned int shader = kv.second;
        glDeleteShader(shader);
    }
    for (const auto& kv : this->shaderPrograms) {
        unsigned int program = kv.second;
        glDeleteProgram(program);
    }
}

/**********************************************************************************/
/**********************************************************************************/

ShaderProgram::ShaderProgram() {
    unsigned int defaultVertexShader, defaultFragmentShader;
    const char * defaultVertexShaderSrc, * defaultFragmentShaderSrc;
    defaultVertexShaderSrc = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vertexColor;\n"
        "void main() {\n"
        " gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0f);\n"
        " vertexColor = aColor;\n"
        "}";
    defaultFragmentShaderSrc = "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        " fragColor = vec4(vertexColor, 1.0f);\n"
        "}";

    // compile default vertex shader
    defaultVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(
        defaultVertexShader, 
        1, 
        &defaultVertexShaderSrc, 
        NULL
    );
    glCompileShader(defaultVertexShader);

    // compile default fragment shader
    defaultFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(
        defaultFragmentShader,
        1,
        &defaultFragmentShaderSrc,
        NULL
    );
    glCompileShader(defaultFragmentShader);

    // add default shaders to shader map
    this->vertexShaders.insert({"", defaultVertexShader});
    this->fragmentShaders.insert({"", defaultFragmentShader});
}

/**********************************************************************************/
/**********************************************************************************/

unsigned int ShaderProgram::loadShader(
    std::string vertexPath,
    std::string fragmentPath
) {
    std::string combinedPath;
    unsigned int shaderProgram;
    int success;

    combinedPath = vertexPath + "::delim::" + fragmentPath;
    
    auto it = this->shaderPrograms.find(combinedPath);
    if (it != this->shaderPrograms.end()) {
        // shader program stored
        shaderProgram = it->second;
        return shaderProgram;
    } else {
        // load, compile, and link shader program from source

        unsigned int vertexShader = this->loadVertexShader(vertexPath);
        unsigned int fragmentShader = this->loadFragmentShader(fragmentPath);
        
        // link shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // add program to list, use the program then returns
        this->shaderPrograms.insert({combinedPath, shaderProgram});
        return shaderProgram;
    }
}

/**********************************************************************************/
/**********************************************************************************/

unsigned int ShaderProgram::loadVertexShader(std::string vertexPath) {
    const char * shaderSrc;
    std::string buffer;
    unsigned int vertexShader;

    auto it = this->vertexShaders.find(vertexPath);
    if (it != this->vertexShaders.end()) { 
        // shader stored
        vertexShader = it->second;
        return vertexShader;
    } else { 
        // load shader from source
        this->openFile(vertexPath, buffer);
        shaderSrc = buffer.c_str();
    }
    
    // compile shader from source
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSrc, NULL);
    glCompileShader(vertexShader);

    // add compiled shader into shader list, return
    this->vertexShaders.insert({vertexPath, vertexShader});
    return vertexShader;
}

/**********************************************************************************/
/**********************************************************************************/

unsigned int ShaderProgram::loadFragmentShader(std::string fragmentPath) {
    const char * shaderSrc;
    std::string buffer;
    unsigned int fragmentShader;

    auto it = this->fragmentShaders.find(fragmentPath);
    if (it != this->fragmentShaders.end()) { 
        // shader stored
        fragmentShader = it->second;
        return fragmentShader;
    } else { 
        // load shader from source
        this->openFile(fragmentPath, buffer);
        shaderSrc = buffer.c_str();
    }
    
    // compile shader from source
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSrc, NULL);
    glCompileShader(fragmentShader);

    // add compiled shader into shader list, return
    this->fragmentShaders.insert({fragmentPath, fragmentShader});
    return fragmentShader;
}

/**********************************************************************************/
/**********************************************************************************/

void ShaderProgram::openFile(const std::string& path, std::string& buffer) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(path);
        std::stringstream strstream;
        strstream << file.rdbuf();
        file.close();
        buffer = strstream.str();
    } catch(const std::exception& e) {
        throw std::runtime_error("File read unsuccessful");
    }
}
