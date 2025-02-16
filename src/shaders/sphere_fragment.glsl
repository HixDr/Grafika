#version 330 core
out vec4 FragColor;

uniform vec3 objectColor; // Set to white in code

void main() {
    FragColor = vec4(objectColor, 1.0); // White color
}