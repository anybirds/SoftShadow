#version 320 es
precision mediump float;

struct Light {
    vec3 _AMBIENT;
    vec3 _DIFFUSE;
    vec3 _SPECULAR;
    vec3 _DIR;
};

in vec3 _FRAG_POS;
in vec3 _FRAG_NORM;
in vec2 _FRAG_UV;

uniform mat4 _CAM;
uniform sampler2D _MAIN_TEX;
uniform Light _LIGHT;
uniform vec3 _AMBIENT;
uniform vec3 _DIFFUSE;
uniform vec3 _SPECULAR;
uniform float _SHININESS;

out vec4 _FRAG_COLOR;

void main() {
    vec3 N = normalize(_FRAG_NORM);
    vec3 L = normalize(_LIGHT._DIR);
    vec3 V = normalize(vec3(_CAM[3]) - _FRAG_POS);
    vec3 H = normalize(V + L);

    vec3 I = clamp(_LIGHT._AMBIENT * _AMBIENT + _LIGHT._DIFFUSE * _DIFFUSE * dot(N, L) + _LIGHT._SPECULAR * _SPECULAR * max(0.0, pow(dot(N, H), _SHININESS)), 0.0, 1.0);
    _FRAG_COLOR = texture(_MAIN_TEX, _FRAG_UV) * vec4(I, 1.0);
}