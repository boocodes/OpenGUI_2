#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform float z_index;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, z_index, 1.0);
    TexCoords = vertex.zw;
}