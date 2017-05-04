#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <gtc\matrix_transform.hpp>
#include <glm.hpp>
#include <assimp\types.h>
#include "shaderPractica.hpp"

using namespace std;
using namespace glm;

struct Vertex {
	vec3 Position;
	vec3 Normal;
	vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh {
public:
	Mesh(vector<Vertex> vert, vector<GLuint> ind, vector<Texture> text);

	void setupMesh();
	void Draw(Shader MeshShader, GLint DrawMode);

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;
};