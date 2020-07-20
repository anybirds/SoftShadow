#version 320 es
precision mediump float;

uniform  vec2 _STRIDE;
uniform sampler2D _VSM;

out vec2 _FRAG_DEPTH;

void main() {
    if (gl_FragCoord.x == 0.0 || gl_FragCoord.y == 0.0 || gl_FragCoord.x == float(textureSize(_VSM, 0).x - 1) || gl_FragCoord.y == float(textureSize(_VSM, 0).y - 1)) {
        // zero edge
        _FRAG_DEPTH = vec2(0.0);
    } else {
        _FRAG_DEPTH =
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 2.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 3.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 4.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 5.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 6.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 7.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 8.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 9.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 10.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 11.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 12.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 13.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 14.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 15.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 16.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 17.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 18.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 19.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 20.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 21.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 22.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 23.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 24.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 25.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 26.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 27.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 28.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 29.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 30.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 31.0 * _STRIDE), 0)) +
        vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 32.0 * _STRIDE), 0));
    }
}