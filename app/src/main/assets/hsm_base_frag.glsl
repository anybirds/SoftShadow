#version 320 es
precision mediump float;

uniform sampler2D _SHADOW_MAP;

out vec2 _FRAG_DEPTH;

void main() {
    float depth = texelFetch(_SHADOW_MAP, ivec2(gl_FragCoord), 0).r;
    _FRAG_DEPTH = vec2(depth, depth);
}