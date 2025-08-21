#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float shadowCalculation(vec4 fragPosLightSpace)
{
    //执行透视分割
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    //从光的角度获得最接近的深度值（使用[0,1]range fragPosLight作为坐标）
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    //从光的角度获取当前片段的深度
    float currentDepth = projCoords.z;
    //检查当前碎片位置是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

void main()
{
     vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
     vec3 normal = normalize(fs_in.Normal);
     vec3 lightColor = vec3(0.3);  
     // 环境光
     vec3 ambient = 0.3 * lightColor;
     // 漫反射光
     vec3 lightDir = normalize(lightPos - fs_in.FragPos);
     float diff = max(dot(lightDir, normal), 0.0);
     vec3 diffuse = diff  * lightColor;
     // 镜面光
     vec3 viewDir = normalize(viewPos - fs_in.FragPos);
     vec3 reflectDir = reflect(-lightDir, normal);
     float spec = 0.0;
     vec3 halfwayDir = normalize(lightDir + viewDir);
     spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
     vec3 specular = spec * lightColor;
     //计算阴影
     float shadow = shadowCalculation(fs_in.FragPosLightSpace);
     vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

     FragColor = vec4(lighting, 1.0);

}