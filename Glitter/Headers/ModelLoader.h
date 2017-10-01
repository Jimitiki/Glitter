#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "glitter.hpp"

#include <string>
#include <vector>

struct mlVertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 UV;
};

struct mlMesh {
	std::string name;
	std::string textureFile;
	std::vector<mlVertex> vertices;
	std::vector<GLuint> indices;
};

// mlModel contains all the loaded data for one model file.
// One model can contain several meshes. This allows each mesh
// to have its own texture.
struct mlModel {
	std::vector<mlMesh> meshes;
};

// Attempts to load the 3d model contained in modelFile. Fills out the modelOut
// struct and returns true on success. Returns false and prints errors on failure.
bool LoadModel(const std::string& directory, const std::string& modelFile, mlModel& modelOut);

#endif