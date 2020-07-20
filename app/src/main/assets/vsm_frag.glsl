#version 320 es
precision mediump float;

uniform  vec2 _STRIDE;
uniform sampler2D _VSM;

out vec2 _FRAG_DEPTH;

void main() {
    _FRAG_DEPTH =
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord), 0)) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + _STRIDE), 0)) * vec2(step(gl_FragCoord.x + _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 2.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 2.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 2.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 3.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 3.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 3.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 4.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 4.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 4.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 5.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 5.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 5.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 6.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 6.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 6.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 7.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 7.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 7.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 8.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 8.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 8.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 9.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 9.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 9.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 10.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 10.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 10.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 11.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 11.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 11.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 12.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 12.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 12.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 13.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 13.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 13.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 14.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 14.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 14.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 15.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 15.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 15.0 * _STRIDE.y, float(textureSize(_VSM, 0).y))) +
    vec2(texelFetch(_VSM, ivec2(gl_FragCoord.xy + 16.0 * _STRIDE), 0)) * vec2(step(gl_FragCoord.x + 16.0 * _STRIDE.x, float(textureSize(_VSM, 0).x)) * step(gl_FragCoord.y + 16.0 * _STRIDE.y, float(textureSize(_VSM, 0).y)));
}