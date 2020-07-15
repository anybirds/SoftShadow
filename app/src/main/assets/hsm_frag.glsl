#version 320 es
precision mediump float;

uniform int _LEVEL;
uniform sampler2D _HSM;

out vec2 _FRAG_DEPTH;

void main() {
    ivec2 coord;
    vec2 depth;
    vec2 temp;

    coord = ivec2(int(gl_FragCoord.x) << 1, int(gl_FragCoord.y) << 1);
    depth = vec2(texelFetch(_HSM, coord, _LEVEL));

    coord.x++;
    temp = vec2(texelFetch(_HSM, coord, _LEVEL));
    depth = vec2(min(temp.x, depth.x), max(temp.y, depth.y));

    coord.y++;
    temp = vec2(texelFetch(_HSM, coord, _LEVEL));
    depth = vec2(min(temp.x, depth.x), max(temp.y, depth.y));

    coord.x--;
    temp = vec2(texelFetch(_HSM, coord, _LEVEL));
    depth = vec2(min(temp.x, depth.x), max(temp.y, depth.y));

    _FRAG_DEPTH = depth;
}
