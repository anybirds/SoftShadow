#version 320 es
precision highp float;
precision highp int;
precision highp sampler2D;

uniform sampler2D _SHADOW_MAP;

out ivec2 _FRAG_DEPTH;

void main() {
    // sample from shadow map
    float depth = texelFetch(_SHADOW_MAP, ivec2(gl_FragCoord), 0).r;

    // clamp to one-sided border effect
    bvec2 inner = equal(gl_FragCoord.xy, max(gl_FragCoord.xy, vec2(1.0)));
    _FRAG_DEPTH = ivec2((-0.5 + vec2(depth, depth * depth)) * 65536.0) * ivec2(inner.x && inner.y);
}