#include "beatCircle.h"

#include <algorithm>
#include <cmath>

BeatCircle::BeatCircle() {
    // setup vertices
    float dTheta = 2 * 3.14 / TRIANGLE_COUNT;
    for (int i = 0; i < TRIANGLE_COUNT; i++) {
        // 3 vertices: center, this, next
        this->indices[3 * i + 0] = 0;
        this->indices[3 * i + 1] = i + 1;
        this->indices[3 * i + 2] = (i + 2 < TRIANGLE_COUNT) ? (i + 2) : 1;

        // essentially (sin(theta), cos(theta))
        // plus 1 due to first one being center
        float theta =  dTheta * i;
        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;
        this->normals[this->stride * (i + 1) + 0] = glm::sin(theta);
        this->normals[this->stride * (i + 1) + 1] = glm::cos(theta);
        this->normals[this->stride * (i + 1) + 2] = 0.0f;
        this->normals[this->stride * (i + 1) + 3] = r;
        this->normals[this->stride * (i + 1) + 4] = g;
        this->normals[this->stride * (i + 1) + 5] = b;
    }
}

float BeatCircle::calculateRadius(float env) {
    env = std::clamp<float>(env, 0.0f, 1.0f);
    float shaped = std::pow(env, 0.6f);
    radius = minRadius + (maxRadius - minRadius) * shaped;
    return radius;
}