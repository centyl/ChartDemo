#version 450 core

layout (location = 0) in vec2 Position;
layout (location = 1) in vec2 Translate;

uniform vec2 windowSize;
uniform float r;

void main() {
    gl_Position = vec4(2 / windowSize * (Position * r + Translate * vec2(1.0, -1.0)) + vec2(-1.0, 1.0), 0.0, 1.0);
}