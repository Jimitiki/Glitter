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



	glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(110.0f), (float) mWidth / (float) mHeight, 0.01f, 100.0f);
	glm::mat4 boxMatrix = matrixTransform(glm::vec3(-0.5f, 0.2f, -1.0f), glm::vec3(0.4f, 0.4f, 0.4f));
	glm::mat4 crayonMatrix = matrixTransform(glm::vec3(0.7f, 0.5f, -3.3f), glm::vec3(2.0f, 2.0f, 2.0f));

	glm::mat4 boxTransform = perspectiveMatrix * boxMatrix;
	glm::mat4 crayonTransform = perspectiveMatrix * crayonMatrix;

	mlModel crayonModel, boxModel;
	if (!LoadModel("D:/Users/Chris/Documents/CS455/Lab4/", "crayon.obj", crayonModel) ||
		!LoadModel("D:/Users/Chris/Documents/CS455/Lab4/", "crayonBox.obj", boxModel))
	{
		return -1;
	}
	mlMesh crayonMesh = crayonModel.meshes.at(0);
	mlMesh boxMesh = boxModel.meshes.at(0);

	int crayonTextureWidth, crayonTextureHeight, crayonChannelCount , boxTextureWidth, boxTextureHeight, boxChannelCount;
	unsigned char *crayonTexture = stbi_load(crayonMesh.textureFile.c_str(), &crayonTextureWidth, &crayonTextureHeight, &crayonChannelCount, 3);
	unsigned char *boxTexture = stbi_load(boxMesh.textureFile.c_str(), &boxTextureWidth, &boxTextureHeight, &boxChannelCount, 3);
	if (!crayonTexture || !boxTexture)
	{
		return -1;
	}

	GLuint shader = LoadProgram("D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.vert",
		"D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.frag");
	
	///////////////INITIALIZE BOX VAO/////////////////
	GLuint boxVAO;
	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);

	GLuint boxVBO;
	glGenBuffers(1, &boxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mlVertex) * boxMesh.vertices.size(), &boxMesh.vertices.at(0), GL_STATIC_DRAW);

	GLuint boxEBO;
	glGenBuffers(1, &boxEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * boxMesh.indices.size(), &boxMesh.indices.at(0), GL_STATIC_DRAW);

	///////////INITIALIZE BOX TEXTURE/////////////////

	GLuint boxTextureID;
	glGenTextures(1, &boxTextureID);
	glBindTexture(GL_TEXTURE_2D, boxTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, boxTextureWidth, boxTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, boxTexture);

	//////////////VERTEX ATTRIB POINTERS///////////////
	GLuint vBoxPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vBoxPosition);
	glVertexAttribPointer(vBoxPosition, 3, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (nullptr));

	GLuint vBoxUV = glGetAttribLocation(shader, "vUV");
	glEnableVertexAttribArray(vBoxUV);
	glVertexAttribPointer(vBoxUV, 2, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (sizeof(float) * 6));




	///////////////INITIALIZE CRAYON VAO/////////////////
	GLuint crayonVAO;
	glGenVertexArrays(1, &crayonVAO);
	glBindVertexArray(crayonVAO);

	GLuint crayonVBO;
	glGenBuffers(1, &crayonVBO);
	glBindBuffer(GL_ARRAY_BUFFER, crayonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mlVertex) * crayonMesh.vertices.size(), &crayonMesh.vertices.at(0), GL_STATIC_DRAW);

	GLuint crayonEBO;
	glGenBuffers(1, &crayonEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crayonEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * crayonMesh.indices.size(), &crayonMesh.indices.at(0), GL_STATIC_DRAW);

	///////////INITIALIZE CRAYON TEXTURE/////////////////

	GLuint crayonTextureID;
	glGenTextures(1, &crayonTextureID);
	glBindTexture(GL_TEXTURE_2D, crayonTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, crayonTextureWidth, crayonTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, crayonTexture);

	//////////////VERTEX ATTRIB POINTERS///////////////
	GLuint vCrayonPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vCrayonPosition);
	glVertexAttribPointer(vCrayonPosition, 3, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (nullptr));

	GLuint vCrayonUV = glGetAttribLocation(shader, "vUV");
	glEnableVertexAttribArray(vCrayonUV);
	glVertexAttribPointer(vCrayonUV, 2, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (sizeof(float) * 6));



	/////////////////OTHER INITIALIZATION///////////////////
	GLint perspectivePositionID = glGetUniformLocation(shader, "perspective");
	GLint transformPositionID = glGetUniformLocation(shader, "transform");

	glEnable(GL_DEPTH_TEST);

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);


		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);

		glBindVertexArray(boxVAO);
		glBindTexture(GL_TEXTURE_2D, boxTextureID);
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &boxTransform[0][0]);

		glDrawElements(GL_TRIANGLES, boxMesh.indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(crayonVAO);
		glBindTexture(GL_TEXTURE_2D, crayonTextureID);
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &crayonTransform[0][0]);

		glDrawElements(GL_TRIANGLES, crayonMesh.indices.size(), GL_UNSIGNED_INT, 0);

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