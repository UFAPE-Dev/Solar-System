#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;

out vec2 TexCoords;
out vec3 vertexColor;
out vec3 vertexNormal;
out vec3 lightDirection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
        vec3 lightPos = vec3(0.0,1.0,0.0);
        vec4 vertexPos = model * vec4(aPos, 1.0);
        TexCoords = aTexCoords;
        gl_Position = projection * view * vertexPos;
        vertexColor = aColor;
        vertexNormal = (model * vec4(aNormal, 0.0)).xyz;
        lightDirection = lightPos - vertexPos.xyz;
}