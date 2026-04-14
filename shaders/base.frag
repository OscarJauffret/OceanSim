#version 410 core
out vec4 FragColor;

in float vHeight;
in vec3 vNormal;
in vec3 vPos;

uniform vec3 deepColor;
uniform vec3 shallowColor;
uniform vec3 viewPos; // Camera position from C++

void main() {
    vec3 N = normalize(vNormal);
    vec3 V = normalize(viewPos - vPos);
    vec3 L = normalize(vec3(0.0, 0.6, -0.8));
    vec3 H = normalize(L + V);

    // Fresnel — this is the master blend
    float cosTheta = max(dot(N, V), 0.0);   // when view aligned with normal, no reflection, when grazing, full reflection
    float F0 = 0.02;
    float fresnel = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

    // Specular sun glint
    float NdotH = max(dot(N, H), 0.0);  // when normal aligned with half vector, max glint
    float sunSpec = pow(NdotH, 300.0); // tighten or loosen to taste
    vec3 sunColor = vec3(237.0/255, 159.0/255, 0) * 3.0; // bright, slightly warm

    // Reflected color: sky + sun glint
    vec3 skyColor = vec3(0.4, 0.6, 0.9); // placeholder until you have a cubemap
    vec3 reflectedColor = skyColor + sunColor * sunSpec;

    // Water body color (transmitted side)
    // Use deepColor/shallowColor but drive blend by fresnel, not height
    vec3 waterColor = mix(shallowColor, deepColor, 0.6); // constant for now

    // Subsurface scattering fake — wave crests glow when backlit
    float waveThin = clamp(vHeight * 0.5 + 0.5, 0.0, 1.0);
    float backlit = max(dot(L, -V), 0.0);
    vec3 sssColor = vec3(0.0, 0.7, 0.5) * waveThin * backlit * 0.3;

    // Final composition: fresnel splits reflection vs transmission
    vec3 finalColor = fresnel * reflectedColor + (1.0 - fresnel) * (waterColor + sssColor);

    // Simple ambient so shadows aren't pure black
    finalColor += vec3(0.01, 0.02, 0.03);

    // Tone mapping (you're adding HDR sun glint, need to compress)
    finalColor = finalColor / (finalColor + vec3(1.0));

    FragColor = vec4(finalColor, 1.0);
}