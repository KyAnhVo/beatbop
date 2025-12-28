#ifndef BEAT_CIRCLE_H
#define BEAT_CIRCLE_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#define TRIANGLE_COUNT 1000
#define PI             3.14
#define BUFFER_STRIDE  6

class BeatCircle {
public:
    float minRadius = 0.5;
    float maxRadius = 0.8;

    // kinematic variables
    float radius = 0.5;
    float velocity = 0;
    float acceleration = 0;

    unsigned int stride = BUFFER_STRIDE;

    // normals for VertexVBO ([xyz][rgb] per value)
    float normals[(TRIANGLE_COUNT + 1) * 6] = {
        0, 0, 0, 0, 0, 0,
    };
    
    // index buffer for EBO ([012][023][034]...)
    unsigned int indices[TRIANGLE_COUNT * 3];

    /* Initialize BeatCircle, set all state values to initial */
    BeatCircle();

    /** Calculate the next radius we use
     * @param env envelope, will be clamped to [0, 1]
     * @returns the radius that should be drawn
     */
    float calculateRadius(float);
};

#endif