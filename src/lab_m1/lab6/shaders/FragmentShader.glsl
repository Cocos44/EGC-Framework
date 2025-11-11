#version 330

in vec3 frag_color;
in vec3 frag_normal;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 normal = normalize(frag_normal);
    vec3 color = frag_color;
    out_color = vec4(color, 1.0);
}
