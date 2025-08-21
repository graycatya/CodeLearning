#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
//这个顶点着色器接收模型矩阵和顶点数据，使用lightSpaceMatrix变换到光源空间中
void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}