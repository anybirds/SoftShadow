#version 320 es
precision highp float;
precision highp int;
precision highp sampler2D;

uniform vec2 _STRIDE;
uniform highp isampler2D _VSM;

out ivec2 _FRAG_DEPTH;

void main() {
    if (int(gl_FragCoord.x) == 0 || int(gl_FragCoord.y) == 0 || int(gl_FragCoord.x) == textureSize(_VSM, 0).x - 1 || int(gl_FragCoord.y) == textureSize(_VSM, 0).y - 1) {
        // zero edge
        _FRAG_DEPTH = ivec2(0);
    } else {
    /*
        vec4 depth =
        texelFetch(_VSM, ivec2(gl_FragCoord), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 2.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 3.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 4.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 5.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 6.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 7.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 8.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 9.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 10.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 11.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 12.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 13.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 14.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 15.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 16.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 17.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 18.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 19.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 20.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 21.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 22.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 23.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 24.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 25.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 26.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 27.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 28.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 29.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 30.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 31.0 * _STRIDE), 0);
        */

        ivec4 depth = texelFetch(_VSM, ivec2(gl_FragCoord), 0);

        depth = depth + texelFetch(_VSM, ivec2(gl_FragCoord.xy - _STRIDE), 0);
        // depth = vec4(depth.r + pow(2.0, clamp(floor(log2(depth.r)) - 10.0, -24.0, 5.0)) * floor(depth.g), mod(depth.g, 1.0), depth.b + pow(2.0, clamp(floor(log2(depth.a)) - 10.0, -24.0, 5.0)) * floor(depth.a), mod(depth.a, 1.0));

        depth = depth + texelFetch(_VSM, ivec2(gl_FragCoord.xy - 2.0 * _STRIDE), 0);
        // depth = vec4(depth.r + pow(2.0, clamp(floor(log2(depth.r)) - 10.0, -24.0, 5.0)) * floor(depth.g), mod(depth.g, 1.0), depth.b + pow(2.0, clamp(floor(log2(depth.a)) - 10.0, -24.0, 5.0)) * floor(depth.a), mod(depth.a, 1.0));

        depth = depth + texelFetch(_VSM, ivec2(gl_FragCoord.xy - 3.0 * _STRIDE), 0);
        // depth = vec4(depth.r + pow(2.0, clamp(floor(log2(depth. r)) - 10.0, -24.0, 5.0)) * floor(depth.g), mod(depth.g, 1.0), depth.b + pow(2.0, clamp(floor(log2(depth.a)) - 10.0, -24.0, 5.0)) * floor(depth.a), mod(depth.a, 1.0));

        _FRAG_DEPTH = ivec2(depth);
        // _FRAG_DEPTH = ivec2(4);
        /*
        depth = depth + texelFetch(_VSM, ivec2(gl_FragCoord.xy - 4.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g), mod(depth.g, 1.0), depth.b + floor(depth.a), mod(depth.a, 1.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 5.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 6.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 7.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 8.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 9.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 10.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 11.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 12.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 13.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 14.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 15.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 16.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 17.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 18.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 19.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 20.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 21.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 22.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 23.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 24.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 25.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 26.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 27.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 28.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 29.0 * _STRIDE), 0) +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 30.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));

        depth = depth +
        texelFetch(_VSM, ivec2(gl_FragCoord.xy - 31.0 * _STRIDE), 0);
        depth = vec4(depth.r + floor(depth.g / 4.0), mod(depth.g, 4.0), depth.b + floor(depth.a / 4.0), mod(depth.a, 4.0));
*/

        // _FRAG_DEPTH = depth;
        //_FRAG_DEPTH = vec4(intBitsToFloat(1), 0.0, 0.0, 1.0);
    }
}