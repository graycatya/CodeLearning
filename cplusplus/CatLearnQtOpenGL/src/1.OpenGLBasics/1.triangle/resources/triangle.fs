#version 330 core
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 outObjectPos;
in vec3 outObjectNormal;
in vec4 outObjectColor;

void main()
{
    float ambientStrength = 0.1;
    //vec3 ambient = ambientStrength * lightColor;

    // diffuse
    //vec3 norm = normalize(outObjectNormal);
    //vec3 lightDir = normalize(lightPos - outObjectPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor;

    //float specularStrength = 0.5;
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;

    //vec4 result = (vec4(ambient, 1.0) + vec4(diffuse, 1.0)) * outObjectColor;
    vec3 L = normalize(lightPos - outObjectPos);
    float NL = max(dot(normalize(outObjectNormal), L), 0.0);
    vec3 col = clamp(outObjectColor.xyz * 0.2 + outObjectColor.xyz * 0.8 * NL, 0.0, 1.0);
    FragColor = vec4(col, 1.0);;
}
