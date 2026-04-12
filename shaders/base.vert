#version 410 core
layout (location = 0) in vec3 aPos;

struct Wave {
    vec2 direction;
    float amplitude;
    float k;
    float c;
};

uniform Wave waves[3]; // Let's start with 3 waves

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;
out float vHeight;

vec3 getGerstner(Wave w, vec3 p) {
    float phase = dot(w.direction, p.xz) * w.k - (uTime * w.c);

    float a = w.amplitude;

    return vec3(
    -w.direction.x * (a / w.k) * sin(phase),
    a * cos(phase),
    -w.direction.y * (a / w.k) * sin(phase)
    );
}


void main() {
    vec3 p = aPos;
    vec3 totalOffset = vec3(0.0);

    for(int i = 0; i < 3; i++) {
        totalOffset += getGerstner(waves[i], p);
    }
    vec3 finalPos = p + totalOffset;
    vHeight = finalPos.y;
    gl_Position = projection * view * model * vec4(finalPos, 1.0);
}