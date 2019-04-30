#ifndef MODEL_H
#define MODEL_H

#include <stb_image.h>
#include <assimp/importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <iostream>
#include "mesh.h"

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

// Model class from LearnOpenGL tutorial
class Model
{
public:
	Model();
	Model(const char *path, bool smooth);
	glm::vec3 getSize();
	glm::vec3 getMin();
	void Draw(Shader shader);

private:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;

	glm::vec3 min;
	glm::vec3 max;

	void loadModel(string path, bool smooth);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, string typeName);
};

#endif