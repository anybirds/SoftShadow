#version 320 es
precision mediump float;

in vec2 _FRAG_UV;

uniform sampler2D _FONT_TEX;
uniform vec4 _COLOR;

out vec4 _FRAG_COLOR;

void main() {
    _FRAG_COLOR = _COLOR * vec4(1.0, 1.0, 1.0, texture(_FONT_TEX, _FRAG_UV).r);
}

