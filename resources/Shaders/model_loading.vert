#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    //Gera o clip = model é a matriz que sofreu operações, projection o campo de visão e view a posição da câmera vec4(aPos, 1.0) é a coordenada do vértice;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}