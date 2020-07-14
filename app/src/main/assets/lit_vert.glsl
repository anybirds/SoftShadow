#version 320 es

layout(location = 0) in vec3 _VERT_POS;
layout(location = 1) in vec3 _VERT_NORM;
layout(location = 2) in vec2 _VERT_UV;

uniform mat4 _MODEL;
uniform mat4 _CAM;
uniform mat4 _NORM;
uniform mat4 _NVM; // Normal Vector Matrix
uniform mat4 _MVP; // Model View Projection

out vec3 _FRAG_POS;
out vec3 _FRAG_NORM;
out vec2 _FRAG_UV;

void main() {
    _FRAG_POS = vec3(_MODEL * vec4(_VERT_POS, 1.0));
    _FRAG_NORM = vec3(_NVM * vec4(_VERT_NORM, 0.0));
    _FRAG_UV = _VERT_UV;
    gl_Position = _MVP * vec4(_VERT_POS, 1.0);
}
