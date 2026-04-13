#version 410 core
out vec4 FragColor;

in float vHeight;
in vec3 vNormal;
in vec3 vPos;

uniform vec3 deepColor;
uniform vec3 shallowColor;
uniform vec3 viewPos; // Camera position from C++

void main() {

    // 2. Lighting Setup
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3)); // Direction of the sun
    vec3 viewDir = normalize(viewPos - vPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    // 3. Diffuse (Base Lighting)
    float diff = max(dot(vNormal, lightDir), 0.0);

    // 4. Specular (The "Sun Glint")
    float spec = pow(max(dot(vNormal, halfDir), 0.0), 32.0);
    vec3 specularColor = vec3(0.8, 0.9, 1.0) * spec;

    // 5. Fresnel Effect (Darker when looking down, brighter/reflective at horizon)
    float fresnel = pow(1.0 - max(dot(vNormal, viewDir), 0.0), 3.0);

    // 6. Final Composition
    float h = clamp((vHeight + 0.8) / 1.6, 0.0, 1.0);
    vec3 baseColor = mix(deepColor, shallowColor, h);

    vec3 finalColor = baseColor * (diff + 0.2); // Add ambient
    finalColor = mix(finalColor, vec3(0.5, 0.7, 0.8), fresnel * 0.5); // Add fresnel
    finalColor += specularColor; // Add the sun glint

    FragColor = vec4(finalColor, 1.0);
}