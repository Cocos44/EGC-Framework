#version 330

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;

out vec3 frag_color;
out vec3 frag_normal;

void main()
{
    frag_color = vertex_color * sin(Time);
    frag_normal = vertex_normal;
    gl_Position = Projection * View * Model * vec4(sin(Time) * vertex_position, 1.0);
}
