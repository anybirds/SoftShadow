#version 320 es
precision mediump float;

in vec2 _FRAG_UV;

uniform sampler2D _UI_TEX;
uniform vec4 _COLOR;

out vec4 _FRAG_COLOR;

void main() {
    _FRAG_COLOR = _COLOR * texture(_UI_TEX, _FRAG_UV);
}

