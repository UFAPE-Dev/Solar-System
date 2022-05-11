#ifndef MODEL
#define MODEL

#include <cmath>
#include <ctime>

#include <GL/freeglut.h>
#include <GL/glext.h>

#include <fstream>
#include <map>
#include <vector>

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include "../Library/gl_texture.h"

class Model {
private:
    static int count_char(std::string &str, char ch) {
        int c = 0;
        int length = (int) str.length() - 1;
        for (int i = 0; i < length; i++) {
            if (str[i] == ch)
                c++;
        }
        return c;
    }

    static bool tem_barra_dupla(std::string &str) {
        int length = (int) str.length() - 2;
        for (int i = 0; i < length; i++) {
            if (str[i] == '/' && str[i + 1] == '/')
                return true;
        }
        return false;
    }

    class Material {
    public:
        float *ambiente;
        float *difusao;
        float *especular;
        GLuint textura;

        Material(float *ambiente, float *difusao, float *especular) {
            this->ambiente = ambiente;
            this->difusao = difusao;
            this->especular = especular;
            this->textura = 0;
        }
    };

    class Face {
    public:
        int arestas;
        int *vertices;
        int *coordenadas_das_texturas;
        int normal;

        Face(int arestas, int *vertices, int *coordenadas_das_texturas, int normal = -1) {
            this->arestas = arestas;
            this->vertices = vertices;
            this->coordenadas_das_texturas = coordenadas_das_texturas;
            this->normal = normal;
        }
    };

    std::string prefix;
    std::vector<Material> materials;
    std::map<std::string, int> mapeamento_dos_materiais;

    std::vector<float *> vertices;
    std::vector<float *> coordenadas_textura;
    std::vector<float *> normais;
    std::vector<Face> faces;

    GLuint list;

    void carregar_material(const char *arquivo) {
        //Tratamento do arquivo
        std::string linha;
        std::vector<std::string> linhas;
        std::ifstream in(arquivo);
        if (!in.is_open()) {
            printf("Impossível carregar o material %s\n", arquivo);
            return;
        }

        while (!in.eof()) {
            std::getline(in, linha);
            linhas.push_back(linha);
        }
        in.close();

        Material *material;
        int quantidade_de_materiais = 0;
        char str[256];
        std::string nome_do_material;
        float *a, *d, *s; //Ambiente, difusão e especular

        for (std::string &temp_linha : linhas) {
            //Se é newmtl estamos adicionado um novo material
            if (temp_linha[0] == 'n' && temp_linha[1] == 'e') {
                sscanf(temp_linha.c_str(), "newmtl %s", str);
                nome_do_material = str;
                mapeamento_dos_materiais[nome_do_material] = quantidade_de_materiais;
                quantidade_de_materiais++;
                a = new float[4]{0.2f, 0.2f, 0.2f, 1.0f};
                d = new float[4]{0.8f, 0.8f, 0.8f, 1.0f};
                s = new float[4]{0.0f, 0.0f, 0.0f, 1.0f};
                materials.emplace_back(a, d, s);
                material = &materials[materials.size() - 1];
            } else if (temp_linha[0] == 'K') {
                switch (temp_linha[1]) {
                    //Ambiente
                    case 'a':
                        sscanf(temp_linha.c_str(), "Ka %f %f %f", &a[0], &a[1], &a[2]);
                        break;
                        //difusão
                    case 'd':
                        sscanf(temp_linha.c_str(), "Kd %f %f %f", &d[0], &d[1], &d[2]);
                        break;
                        //Especular
                    case 's':
                        sscanf(temp_linha.c_str(), "Ks %f %f %f", &s[0], &s[1], &s[2]);
                        break;
                }
            } else if (temp_linha[0] == 'm' && temp_linha[1] == 'a') {
                //Carregando a imagem
                sscanf(temp_linha.c_str(), "map_Kd %s", str);
                std::string file = prefix + str;
                Image imagem;
                Load_Texture_Swap(&imagem, file.c_str());
                //Define o nome da textura
                glGenTextures(1, &(material->textura));
                //Usa a textura
                glBindTexture(GL_TEXTURE_2D, material->textura);
                //Define os parâmetros da textura
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                //Faz o carregamento da textura
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) imagem.w, (GLsizei) imagem.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagem.img);
                //Reseta a textura para usar o material padrão
                glBindTexture(GL_TEXTURE_2D, 0);
                //Deleta a imagem da memória
                Delete_Image(&imagem);
            }
        }
    }

    void adicionar_face_3vertices(std::string &line) {
        int v0, v1, v2;
        sscanf(line.c_str(), "f %d %d %d", &v0, &v1, &v2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.emplace_back(3, v, nullptr);
    }

    void adicionar_face_3textura(std::string &line) {
        int v0, v1, v2, t0, t1, t2;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.emplace_back(3, v, t);
    }

    void adicionar_face_3textura_normal_opcional(std::string &line) {
        int v0, v1, v2, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        faces.emplace_back(3, v, nullptr, n - 1);
    }

    void adicionar_face_3textura_com_normal(std::string &line) {
        int v0, v1, v2, t0, t1, t2, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n);
        int *v = new int[3]{v0 - 1, v1 - 1, v2 - 1};
        int *t = new int[3]{t0 - 1, t1 - 1, t2 - 1};
        faces.emplace_back(3, v, t, n - 1);
    }

    void adicionar_face_4vertices(std::string &line) {
        int v0, v1, v2, v3;
        sscanf(line.c_str(), "f %d %d %d %d", &v0, &v1, &v2, &v3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.emplace_back(4, v, nullptr);
    }

    void adicionar_face_4textura(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3;
        sscanf(line.c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v0, &t0, &v1, &t1, &v2, &t2, &v3, &t3);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.emplace_back(4, v, t);
    }

    void adicionar_face_4textura_normal_opcional(std::string &line) {
        int v0, v1, v2, v3, n;
        sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v0, &n, &v1, &n, &v2, &n, &v3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        faces.emplace_back(4, v, nullptr, n - 1);
    }

    void adicionar_face_4textura_com_normal(std::string &line) {
        int v0, v1, v2, v3, t0, t1, t2, t3, n;
        sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &t0, &n, &v1, &t1, &n, &v2, &t2, &n, &v3,
               &t3, &n);
        int *v = new int[4]{v0 - 1, v1 - 1, v2 - 1, v3 - 1};
        int *t = new int[4]{t0 - 1, t1 - 1, t2 - 1, t3 - 1};
        faces.emplace_back(4, v, t, n - 1);
    }

public:
    float pos_x, pos_y, pos_z;

    void load(const char *arquivo) {
        //Tratamento do arquivo
        std::string temp_arquivo = arquivo;
        prefix = "";
        int n = temp_arquivo.find_last_of('/') + 1;
        if (n > 0)
            prefix = temp_arquivo.substr(0, n);

        std::string linha;
        std::vector<std::string> linhas;
        std::ifstream in(arquivo);
        if (!in.is_open()) {
            printf("Impossível abrir arquivo %s\n", arquivo);
            return;
        }

        while (!in.eof()) {
            std::getline(in, linha);
            linhas.push_back(linha);
        }
        in.close();

        //Início da coleta de dados
        float a, b, c; //Variáveis das coordenadas
        char str[256];

        pos_x = pos_y = 0.0f;

        float sum_x = 0.0f, sum_y = 0.0f, sum_z = 0.0f;

        for (std::string &temp_linha : linhas) {
            if (temp_linha[0] == 'v') {
                //Vértices
                if (temp_linha[1] == ' ') {
                    sscanf(temp_linha.c_str(), "v %f %f %f", &a, &b, &c);
                    if (a > 0.0f)
                        sum_x += a;
                    else
                        sum_x -= a;
                    if (b > 0.0f)
                        sum_y += b;
                    else
                        sum_y -= b;
                    if (c > 0.0f)
                        sum_z += c;
                    else
                        sum_z -= c;
                    pos_x += a;
                    pos_y += b;
                    vertices.push_back(new float[3]{a, b, c});
                    //Vértices das texturas
                } else if (temp_linha[1] == 't') {
                    sscanf(temp_linha.c_str(), "vt %f %f", &a, &b);
                    coordenadas_textura.push_back(new float[2]{a, b});
                    //Lista dos vetores normais
                } else {
                    sscanf(temp_linha.c_str(), "vn %f %f %f", &a, &b, &c);
                    normais.push_back(new float[3]{a, b, c});
                }
            } else if (temp_linha[0] == 'f') {
                int arestas = count_char(temp_linha, ' ');
                int quantidade_de_barras = count_char(temp_linha, '/');
                //Se não tem barra é aresta de face
                if (quantidade_de_barras == 0) {
                    if (arestas == 3)
                        adicionar_face_3vertices(temp_linha);
                    else
                        adicionar_face_4vertices(temp_linha);
                    //Se tem barra e as barras são iguais a quantidade de vertices é aresta de textura
                } else if (quantidade_de_barras == arestas) {
                    if (arestas == 3)
                        adicionar_face_3textura(temp_linha);
                    else
                        adicionar_face_4textura(temp_linha);
                    //Se tem barra e as barras são o dobro da quantidade de vertices é vetor de normal de uma face
                } else if (quantidade_de_barras == arestas * 2) {
                    //Se tem barra duplas não é utilizado textura
                    if (tem_barra_dupla(temp_linha)) {
                        if (arestas == 3)
                            adicionar_face_3textura_normal_opcional(temp_linha);
                        else
                            adicionar_face_4textura_normal_opcional(temp_linha);
                    } else {
                        if (arestas == 3)
                            adicionar_face_3textura_com_normal(temp_linha);
                        else
                            adicionar_face_4textura_com_normal(temp_linha);
                    }
                }
                //Tratamento do material mtllib
            } else if (temp_linha[0] == 'm' && temp_linha[1] == 't') {
                sscanf(temp_linha.c_str(), "mtllib %s", str);
                std::string file = prefix + str;
                carregar_material(file.c_str());
            } else if (temp_linha[0] == 'u' && temp_linha[1] == 's') {
                sscanf(temp_linha.c_str(), "usemtl %s", str);
                std::string material = str;
                if (mapeamento_dos_materiais.find(material) != mapeamento_dos_materiais.end())
                    faces.emplace_back(-1, nullptr, nullptr, mapeamento_dos_materiais[material]);
            }
        }

        bool possui_textura = false;

        //Gera uma lista de exibição
        list = glGenLists(1);
        glNewList(list, GL_COMPILE);
        for (Face &face : faces) {
            //Iluminação da face
            if (face.arestas == -1) {
                possui_textura = false;
                glLightfv(GL_LIGHT0, GL_AMBIENT, materials[face.normal].ambiente);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, materials[face.normal].difusao);
                glLightfv(GL_LIGHT0, GL_SPECULAR, materials[face.normal].especular);
                if (materials[face.normal].textura != 0) {
                    possui_textura = true;
                    glBindTexture(GL_TEXTURE_2D, materials[face.normal].textura);
                }
                continue;
            }

            //Define o vetor normal atual
            if (face.normal != -1)
                glNormal3fv(normais[face.normal]);
            else
                glDisable(GL_LIGHTING);


            //Desenho do modelo
            if (possui_textura) {
                glBegin(GL_POLYGON);
                for (int i = 0; i < face.arestas; i++) {
                    //Define as coordenadas da textura para o vétice atual
                    glTexCoord2fv(coordenadas_textura[face.coordenadas_das_texturas[i]]);
                    //Desenho do vertice
                    glVertex3fv(vertices[face.vertices[i]]);
                }
                glEnd();
            } else {
                glBegin(GL_POLYGON);
                for (int i = 0; i < face.arestas; i++)
                    //Desenho do vertice
                    glVertex3fv(vertices[face.vertices[i]]);
                glEnd();
            }
        }
        glEndList();

        printf("Model: %s\n", arquivo);
        printf("Vertices: %zu\n", vertices.size());
        printf("Texcoords: %zu\n", coordenadas_textura.size());
        printf("Normals: %zu\n", normais.size());
        printf("Faces: %zu\n", faces.size());
        printf("Materials: %zu\n", materials.size());

        sum_x /= vertices.size();
        sum_y /= vertices.size();
        sum_z /= vertices.size();
        pos_x /= vertices.size();
        pos_x = -pos_x;
        pos_y /= vertices.size();
        pos_y = -pos_y;
        pos_z = -sqrt(sum_x * sum_x + sum_y * sum_y + sum_z * sum_z) * 15;

        printf("Pos_X: %f\n", pos_x);
        printf("Pos_Y: %f\n", pos_y);
        printf("Pos_Z: %f\n", pos_z);

        //Deleta os materiais da memória
        for (Material &material : materials) {
            delete material.ambiente;
            delete material.difusao;
            delete material.especular;
        }

        materials.clear();
        mapeamento_dos_materiais.clear();

        for (float *f : vertices)
            delete f;
        vertices.clear();
        for (float *f : coordenadas_textura)
            delete f;
        coordenadas_textura.clear();
        for (float *f : normais)
            delete f;
        normais.clear();
        faces.clear();
    }

    void draw() { glCallList(list); }
};

#endif