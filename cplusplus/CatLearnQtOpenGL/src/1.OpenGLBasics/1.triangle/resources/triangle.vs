#version 330 core
layout (location = 0) in vec4 objectPos;
layout (location = 1) in vec4 objectColor;
layout (location = 2) in vec3 objectNormal;

uniform mat4 projectionMatrix;
uniform mat4 mvpMatrix;
uniform mat3 normalMatrix;

out vec3 outObjectPos;
out vec3 outObjectNormal;
out vec4 outObjectColor;

void main()
{
   outObjectPos = objectPos.xyz;
   outObjectColor = objectColor;
   outObjectNormal = objectNormal;
   gl_Position = projectionMatrix * mvpMatrix * objectPos;

}
