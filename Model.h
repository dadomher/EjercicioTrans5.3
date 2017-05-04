#include "Mesh.h"
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "shaderPractica"

class Model {
public:
	Model(GLchar* path);

	void Draw(Shader shader, GLint drawMode)

};