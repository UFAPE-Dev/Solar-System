#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//Especifica que a textura é 2D
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
    //A função textura faz o mapeamento da textura utilizando a coordenada especificada,
    //A saída é a respectiva cor com base na imagem.
    FragColor = texture(texture_diffuse1, TexCoords);
}