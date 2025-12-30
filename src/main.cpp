#include <algorithm>
#include <atomic>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <thread>

#include <iostream>

#include "beatCircle.h"
#include "shader.h"
#include "buffers.h"
#include "sound.h"

#define GAMMA_MAX 32768.0f
#define GAMMA_MIN 8.0f

#define IS_DEBUG  true

float aspectRatio;
float gain;
bool minusNotPressed = true;
bool plusNotPressed = true;

void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);
void worker();

int main(int argc, char** argv)
{
    BeatCircle circle;
    
    std::thread soundThreat(worker);

    // version hinting
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Beatbop 0.0.1", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initiate GLAD" << std::endl;
        return -1;
    }

    // viewport, callback
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    aspectRatio = float(w) / h;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Shader
    ShaderProgram& shader = ShaderProgram::instance();
    unsigned int program = shader.loadShader("../shader/basic.vert", "../shader/basic.frag");
    glUseProgram(program);

    // buffers
    Buffers buffer(0, 0, 0, circle.stride * sizeof(float));
    glBindVertexArray(buffer.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.VertexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // buffer vertices
    glBufferData(
        GL_ARRAY_BUFFER, 
        sizeof(circle.normals), 
        circle.normals, 
        GL_STATIC_DRAW
    );

    // buffer indices
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(circle.indices),
        circle.indices,
        GL_STATIC_DRAW
    );

    // attribute pointer (normal)
    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        buffer.stride, 
        (void*) 0
    );

    // attribute pointer (color)
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        buffer.stride,
        (void*)(3 * sizeof(float))
    );
    
    int radiusUniformLocation = glGetUniformLocation(program, "radius");
    int aspectRatioUniformLocation = glGetUniformLocation(program, "aspectRatio");

    while (!Sound::globalSound.load(std::memory_order_relaxed)) ;
    Sound * sound = Sound::globalSound.load(std::memory_order_relaxed);

    gain = 128;
    glfwSetKeyCallback(window, key_callback);

    while(!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(buffer.VAO);

        // set aspectRatio uniform
        glUniform1f(aspectRatioUniformLocation, aspectRatio);

        // set radius uniform
        float env = sound->env.load(std::memory_order_relaxed) * gain;
        float r = circle.calculateRadius(env);
        glUniform1f(radiusUniformLocation, r);

        glDrawElements(GL_TRIANGLES, TRIANGLE_COUNT * 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // done

    sound->close.store(true, std::memory_order_relaxed);
    soundThreat.join();
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    aspectRatio = float(width) / height;
}

void worker() {
    Sound::instance();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        gain = std::min(GAMMA_MAX, gain * 2);
    }
    if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        gain = std::max(GAMMA_MIN, gain / 2);
    }
}

