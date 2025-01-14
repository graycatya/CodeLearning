#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 mvp_matrix;

out vec4 ourColor;
void main()
{
   gl_Position = mvp_matrix * aPos;
   ourColor = aColor;
}
