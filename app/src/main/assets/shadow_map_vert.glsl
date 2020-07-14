#version 320 es

layout(location = 0) in vec3 _VERT_POS;

uniform mat4 _MVP;

void main() {
    gl_Position = _MVP * vec4(_VERT_POS, 1.0);
}