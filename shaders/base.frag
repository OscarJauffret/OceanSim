#version 410 core
out vec4 FragColor;
in float vHeight; // Received from Vertex Shader

void main() {
    // 1. Define Ocean Colors
    vec3 deepWater = vec3(0.007, 0.039, 0.066); // Dark Navy
    vec3 shallowWater = vec3(0.0, 0.439, 0.501); // Teal

    // 2. Normalize height for coloring
    // If your max amplitude is 0.5, map -0.5 -> 0.5 to 0.0 -> 1.0
    float intensity = clamp((vHeight + 0.5) / 1.0, 0.0, 1.0);

    // 3. Simple Shading
    vec3 finalColor = mix(deepWater, shallowWater, intensity);

    // 4. Add "Foam" peaks (Brighter highlights)
    if(vHeight > 0.35) {
        finalColor += vec3(0.2);
    }

    FragColor = vec4(finalColor, 1.0);
}