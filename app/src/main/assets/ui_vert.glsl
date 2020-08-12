#version 320 es

layout (location = 0) in vec2 _VERT_POS;
layout (location = 1) in vec2 _VERT_UV;

out vec2 _FRAG_UV;

uniform mat4 _NORM;

void main() {
    gl_Position = _NORM * vec4(_VERT_POS, 0.0, 1.0);
    _FRAG_UV = _VERT_UV;
}