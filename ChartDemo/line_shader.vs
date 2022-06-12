#version 450 core

layout (location = 0) in vec2 Position;

uniform vec2 windowSize;

void main() {
    gl_Position = vec4((Position * 2.0 / windowSize - 1.0) * vec2(1.0, -1.0), 0.0, 1.0);
}