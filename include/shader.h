#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>

class ShaderProgram {
    /* A hashmap keyed by path, valued by vertex shader id */
    std::unordered_map<std::string, unsigned int> vertexShaders;

    /* A hashmap keyed by path, valued by vertex shader id */
    std::unordered_map<std::string, unsigned int> fragmentShaders;

    /* A hashmap keyed by 2 paths, valued by shader program id */
    std::unordered_map<std::string, unsigned int> shaderPrograms;

public:
    /* Get the singleton instance of shaderProgram */
    static ShaderProgram& instance();

    /* Destructor */
    ~ShaderProgram();

    /** Use the shader program from given paths
     *
     * @param vertexPath the path to vertex shader.
     *      if empty string, use default shader.
     * @param fragmentPath the path to fragment shader.
     *      if empty string, use default shader.
     * @returns ID of the shader program.
     */
    unsigned int loadShader(std::string, std::string);

    /** Compile the vertex shader from given path
     *
     * @param vertexPath the path to vertex shader.
     *      if empty string, use default shader.
     * @returns ID of the vertex shader.
     */
    unsigned int loadVertexShader(std::string);

    /** Compile the fragment shader from given path
     *
     * @param fragmentPath the path to fragment shader.
     *      if empty string, use default shader.
     * @returns ID of the fragment shader.
     */
    unsigned int loadFragmentShader(std::string);

private:
    ShaderProgram();
    void openFile(const std::string&, std::string&);

public:
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
};

#endif