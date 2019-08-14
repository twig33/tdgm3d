#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;

out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 proj;
uniform mat4 view;
uniform mat4 transform;

void main()
{
    gl_Position = proj * view * transform * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(aColor, 1.0);
}