// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>

glm::mat4 matrixTransform(glm::vec3 const& translate, glm::vec3 const& scale);
void initializeVAO(GLuint vao, GLuint vbo, GLuint ebo, float *positions, float *uvs, unsigned int *indices);
void initializeTexture(GLuint texture, unsigned char *loadedTexture, int width, int height);


int main(int argc, char * argv[])
{



	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr)
	{
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	mlModel crayonModel, boxModel;
	if (!LoadModel("D:/Users/Chris/Documents/CS455/Lab4/", "crayon.obj", crayonModel) ||
		!LoadModel("D:/Users/Chris/Documents/CS455/Lab4/", "crayonBox.obj", boxModel))
	{
		return -1;
	}
	mlMesh crayonMesh = crayonModel.meshes.at(0);
	mlMesh boxMesh = boxModel.meshes.at(0);

	int crayonTextureWidth, crayonTextureHeight, crayonChannelCount, boxTextureWidth, boxTextureHeight, boxChannelCount;
	unsigned char *crayonTexture = stbi_load(crayonMesh.textureFile.c_str(), &crayonTextureWidth, &crayonTextureHeight, &crayonChannelCount, 3);
	unsigned char *boxTexture = stbi_load(boxMesh.textureFile.c_str(), &boxTextureWidth, &boxTextureHeight, &boxChannelCount, 3);
	if (!crayonTexture || !boxTexture)
	{
		return -1;
	}

	GLuint shader = LoadProgram("D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.vert",
		"D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.frag");

	/////////RETRIEVE CRAYON OBJECT DATA//////////
	float crayonPositions[1584 * 3];
	float crayonUVs[1584 * 2];
	unsigned int crayonIndices[2376];
	for (int i = 0; i < crayonMesh.vertices.size(); i++)
	{
		int pIndex = i * 3;
		crayonPositions[pIndex] = crayonMesh.vertices.at(i).position.x;
		crayonPositions[pIndex + 1] = crayonMesh.vertices.at(i).position.y;
		crayonPositions[pIndex + 2] = crayonMesh.vertices.at(i).position.z;
	}
	for (int i = 0; i < crayonMesh.vertices.size(); i++)
	{
		int uIndex = i * 2;
		crayonUVs[uIndex] = crayonMesh.vertices.at(i).UV.x;
		crayonUVs[uIndex + 1] = crayonMesh.vertices.at(i).UV.y;
	}
	for (int i = 0; i < crayonMesh.indices.size(); i++)
	{
		crayonIndices[i] = crayonMesh.indices.at(i);
	}

	/*////////RETRIEVE CRAYON BOX OBJECT DATA/////////
	float boxPositions[56 * 3];
	float boxUVs[56 * 2];
	unsigned int boxIndices[84];
	for (int i = 0; i < boxMesh.vertices.size(); i++)
	{
		int pIndex = i * 3;
		boxPositions[pIndex] = boxMesh.vertices.at(i).position.x;
		boxPositions[pIndex + 1] = boxMesh.vertices.at(i).position.y;
		boxPositions[pIndex + 2] = boxMesh.vertices.at(i).position.z;
	}
	for (int i = 0; i < boxMesh.vertices.size(); i++)
	{
		int uIndex = i * 2;
		boxUVs[uIndex] = boxMesh.vertices.at(i).UV.x;
		boxUVs[uIndex + 1] = boxMesh.vertices.at(i).UV.y;
	}
	for (int i = 0; i < boxMesh.indices.size(); i++)
	{
		boxIndices[i] = boxMesh.indices.at(i);
	}*/

	GLuint VAOs[2];
	glGenVertexArrays(1, VAOs);
	GLuint VBOs[2];
	glGenBuffers(2, VBOs);
	GLuint EBOs[2];
	glGenBuffers(2, EBOs);

	///////////////INITIALIZE CRAYON VAO/////////////////
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crayonPositions) + sizeof(crayonUVs), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(crayonPositions), crayonPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(crayonPositions), sizeof(crayonUVs), crayonUVs);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crayonIndices), crayonIndices, GL_STATIC_DRAW);


	/*//////////////INITIALIZE CRAYON BOX VAO/////////////////
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxPositions) + sizeof(boxUVs), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(boxPositions), boxPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(boxPositions), sizeof(boxUVs), boxUVs);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) (nullptr));
	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid *) sizeof(boxPositions));


	*///////////INITIALIZE CRAYON TEXTURE/////////////////
	GLuint textures[2];
	glGenTextures(2, textures);

	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, crayonTextureHeight, crayonTextureWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, crayonTexture);

	/*//////////INITIALIZE CRAYON BOX TEXTURE/////////////////
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, boxTextureWidth, boxTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, boxTexture);*/

	glm::mat4 p = glm::perspective(glm::radians(90.0f), (float) mWidth / (float) mHeight, 0.01f, 100.0f);
	GLint perspectivePositionID = glGetUniformLocation(shader, "perspective");

	glm::mat4 crayonTransform = matrixTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	GLint transformPositionID = glGetUniformLocation(shader, "transform");

	GLuint vPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) (nullptr));
	GLuint vUV = glGetAttribLocation(shader, "vUV");
	glEnableVertexAttribArray(vUV);
	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid *) sizeof(crayonPositions));

	//t = p * t;

	glEnable(GL_DEPTH_TEST);

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);

		//glBindVertexArray(VAOs[0]);
		//glBindTexture(GL_TEXTURE_2D, textures[0]);
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &crayonTransform[0][0]);

		glDrawElements(GL_TRIANGLES, 2376, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(VAOs[1]);
		//glBindTexture(GL_TEXTURE_2D, textures[1]);
		//glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);
		

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();

	return EXIT_SUCCESS;
}

glm::mat4 matrixTransform(glm::vec3 const& translate, glm::vec3 const& scale)
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), translate);
	return translateMatrix * scaleMatrix;
}