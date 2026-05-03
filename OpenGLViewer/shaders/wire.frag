#version 330 core

out vec4 FragColor;

uniform bool uSelected;

void main() {
    // Selected objects get a yellow wire, others get a dim gray
    if (uSelected)
        FragColor = vec4(1.0, 0.9, 0.1, 1.0);
    else
        FragColor = vec4(0.25, 0.25, 0.25, 1.0);
}
