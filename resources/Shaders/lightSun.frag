#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 vertexNormal;
in vec3 lightDirection;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 normalVector = normalize(vertexNormal);
    vec3 lightVector = normalize(lightDirection);
    float dotProduct = dot(normalVector, lightVector);
    float brightness = max(dotProduct, 0.38);
    vec3 diffuse = brightness * lightColor;
    FragColor = vec4(diffuse, 1.0) * texture(texture_diffuse1, TexCoords);

}