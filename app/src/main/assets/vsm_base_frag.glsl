#version 320 es
precision mediump float;

uniform sampler2D _SHADOW_MAP;

out vec2 _FRAG_DEPTH;

void main() {
    if (gl_FragCoord.x == 0.0 || gl_FragCoord.y == 0.0 || gl_FragCoord.x == float(textureSize(_SHADOW_MAP, 0).x + 1) || gl_FragCoord.y == float(textureSize(_SHADOW_MAP, 0).y + 1)) {
        // zero edge
        _FRAG_DEPTH = vec2(0.0);
    } else {
        float depth = texelFetch(_SHADOW_MAP, ivec2(gl_FragCoord) - ivec2(1), 0).r;
        _FRAG_DEPTH = vec2(depth * 0.01, depth * depth * 0.01);
    }
}