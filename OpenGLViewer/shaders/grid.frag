#version 330 core

in  float vDist;
out vec4  FragColor;

void main() {
    // Fade grid with distance
    float alpha = clamp(1.0 - vDist / 20.0, 0.0, 1.0);
    FragColor = vec4(0.4, 0.4, 0.45, alpha * 0.5);
}
