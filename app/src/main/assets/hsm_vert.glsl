#version 320 es

void main() {
    float x = float(((gl_VertexID + 1) / 3) % 2);
    float y = float(((gl_VertexID + 2) / 3) % 2);
    gl_Position = vec4(-1.0 + x * 2.0, -1.0 + y * 2.0, 0.0, 1.0);
}