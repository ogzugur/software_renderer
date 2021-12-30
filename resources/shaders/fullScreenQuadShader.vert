#version 330 core

vec3 gridPlane[6] = vec3[] (vec3(1.0f, 1.0f, -1.0), vec3(-1.0f, 1.0f, -1.0), vec3(-1.0f, -1.0f, -1.0), vec3(-1.0f, -1.0f, -1.0), vec3(1.0f, -1.0f, -1.0), vec3(1.0f, 1.0f, -1.0));
out vec2 texCoord;
void main() {
    texCoord = (gridPlane[gl_VertexID].xy + 1.0)/2;
    gl_Position = vec4(gridPlane[gl_VertexID], 1.0f);
}