#ifndef BUFFERS_H
#define BUFFERS_H

#include <glad/glad.h>

class Buffers {

public:
    /* Vertex "constant" VBO: [xyz][rgb] */
    unsigned int VertexVBO;

    /* Vertex array object */
    unsigned int VAO;

    /* Element buffer object: for angle indexing */
    unsigned int EBO;

    /* Stride: how much difference from a vertex to the next in memory in VBO */
    unsigned int stride;

public:
    /** Creates a Buffers object.
     *
     * @param VBO the VBO id. If 0, create a new VBO and assign
     *       to this object.
     * @param EBO the EBO id. Same as above.
     */
    Buffers(unsigned int, unsigned int, unsigned int, unsigned int);

    /** Use this Buffers object for any subsequent "draws" */
    void useBuffers();
};

#endif
