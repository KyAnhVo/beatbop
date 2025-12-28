#version 330 core
layout (location = 0) in vec3   pos;
layout (location = 1) in vec3   color;

uniform float aspectRatio;
uniform float radius;
out vec4 vertexColor;

const float PI = radians(180);

float toGoalRadius(float);

void main() {
    // scale factor
    float scaleX = min(1.0, 1.0/aspectRatio);
    float scaleY = min(1.0, aspectRatio);
    vec2 scaler = vec2(scaleX, scaleY);

    // current radius from goal radius and 
    gl_Position = vec4(pos.xy * scaler * radius, pos.z, 1.0);
    vertexColor = vec4(color, 1.0);
}