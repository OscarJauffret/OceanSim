#version 410 core
layout (location = 0) in vec3 aPos;

#define MAX_WAVES 900

uniform vec4 waves[MAX_WAVES];  // x=theta, y=amplitude, z=k, w=omega. Use a struct so less uniforms are needed

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;
uniform float numberOfWaves;

out float vHeight;
out vec3 vNormal;
out vec3 vPos;
out float vJacobian;

void main() {
    vec3 p0 = aPos;
    vec3 totalOffset = vec3(0.0);
    vec3 tangent = vec3(1.0, 0.0, 0.0);
    vec3 binormal = vec3(0.0, 0.0, 1.0);

    int activeWaves = int(clamp(numberOfWaves, 0.0, float(MAX_WAVES)));

    for(int i = 0; i < activeWaves; i++) {
        vec4 w = waves[i];
        vec2 waveVector = vec2(w.z * cos(w.x), w.z * sin(w.x)); // Unit vector in the wave direction
        float phase = (w.w * uTime) - (dot(waveVector, p0.xz)); // Phase of the wave at position p0

        float sinP = sin(phase);
        float cosP = cos(phase);
        float cosTheta = cos(w.x);
        float sinTheta = sin(w.x);

        totalOffset += vec3(
            w.y * cosTheta * sinP, // X offset
            w.y * cosP,                 // Y offset
            w.y * sinTheta * sinP  // Z offset
        );

        // Partial derivatives for the Tangent (Rate of change along X-axis 'x0')
        // dX/dx0, dY/dx0, dZ/dx0
        tangent.x -= w.y * cosTheta * cosP * waveVector.x;  // h cos(theta) cos(P) k cos(theta)
        tangent.y += w.y * sinP * waveVector.x;             // h sin(P) k cos(theta)
        tangent.z -= w.y * sinTheta * cosP * waveVector.x;  // h sin(theta) cos(P) k cos(theta)

        // Partial derivatives for the Binormal (Rate of change along Z-axis 'z0')
        // dX/dz0, dY/dz0, dZ/dz0
        binormal.x -= w.y * cosTheta * cosP * waveVector.y;  // h cos(theta) cos(P) k sin(theta)
        binormal.y += w.y * sinP * waveVector.y;             // h sin(P) k sin(theta)
        binormal.z -= w.y * sinTheta * cosP * waveVector.y;  // h sin(theta) cos(P) k sin(theta)
    }
    vec3 finalPos = p0 + totalOffset;
    vHeight = finalPos.y;
    vNormal = normalize(cross(binormal, tangent)); // Cross product to get the normal vector
    vec4 worldPos = model * vec4(finalPos, 1.0);
    vPos = worldPos.xyz;
    vJacobian = tangent.x * binormal.z - tangent.z * binormal.x;
    gl_Position = projection * view * worldPos;
}