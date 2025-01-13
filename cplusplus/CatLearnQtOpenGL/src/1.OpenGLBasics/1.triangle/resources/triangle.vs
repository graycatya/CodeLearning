#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 mvp_matrix;

out vec4 ourColor;
void main()
{
   gl_Position = mvp_matrix * vec4(aPos, 1.0);
   ourColor = aColor * vec4(0.0, 0.0, 0.0, 1.0);
}
