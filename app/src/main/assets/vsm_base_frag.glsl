#version 320 es
precision mediump float;

uniform sampler2D _SHADOW_MAP;

out ivec4 _FRAG_DEPTH;

void main() {
    if (int(gl_FragCoord.x) == 0 || int(gl_FragCoord.y) == 0 || int(gl_FragCoord.x) == textureSize(_SHADOW_MAP, 0).x + 1 || int(gl_FragCoord.y) == textureSize(_SHADOW_MAP, 0).y + 1) {
        // zero edge
        _FRAG_DEPTH = ivec4(0.0);
    } else {
        float depth = texelFetch(_SHADOW_MAP, ivec2(gl_FragCoord) - ivec2(1), 0).r;
        //_FRAG_DEPTH = vec4(0.0, -0.8975 + depth, 0.0, -0.8975 + depth * depth);
        _FRAG_DEPTH = ivec4(16);
    }
}