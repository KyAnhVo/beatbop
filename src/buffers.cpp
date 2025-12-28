#include "buffers.h"

Buffers::Buffers(
    unsigned int VertexVBO,
    unsigned int VAO, 
    unsigned int EBO,
    unsigned int stride
) {
    if (VertexVBO != 0) {
        this->VertexVBO = VertexVBO;
    } else {
        glGenBuffers(1, &(this->VertexVBO));
    }

    if (VAO != 0) {
        this->VAO = VAO;
    } else {
        glGenVertexArrays(1, &(this->VAO));
    }

    if (EBO != 0) {
        this->EBO = EBO;
    } else {
        glGenBuffers(1, &(this->EBO));
    }

    this->stride = stride;
}

void Buffers::useBuffers() {
    glBindVertexArray(this->VAO);
}