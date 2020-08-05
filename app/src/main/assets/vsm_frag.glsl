#version 320 es
precision highp float;
precision highp int;
precision highp sampler2D;

uniform vec2 _STRIDE;
uniform highp isampler2D _VSM;

out ivec2 _FRAG_DEPTH;

void main() {
    // sample from intermediate vsm texture
    ivec2 depth = texelFetch(_VSM, ivec2(gl_FragCoord), 0).xy
    + texelFetch(_VSM, ivec2(gl_FragCoord.xy - _STRIDE), 0).xy
    + texelFetch(_VSM, ivec2(gl_FragCoord.xy - 2.0 * _STRIDE), 0).xy
    + texelFetch(_VSM, ivec2(gl_FragCoord.xy - 3.0 * _STRIDE), 0).xy;

    _FRAG_DEPTH = depth;
}