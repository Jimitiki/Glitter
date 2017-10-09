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

glm::mat4 matrixTransform(glm::vec3 const& translate, glm::vec3 const& scale, glm::vec3 const& rotate);

int main(int argc, char * argv[])
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(1920, 1080, "OpenGL", nullptr, nullptr);

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


	glm::mat4 transformMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(60.0f), (float) mWidth / (float) mHeight, 0.01f, 100.0f);

	mlModel model;
	if (!LoadModel("D:/Users/Chris/Documents/CS455/Lab5/", "mech.obj", model))
	{
		return -1;
	}

	mlMesh *curMesh;
	int vertexCount = 0, indexCount = 0;

	for (int i = 0; i < model.meshes.size(); i++)
	{
		curMesh = &model.meshes.at(i);
		vertexCount += curMesh->vertices.size();
		indexCount += curMesh->indices.size();
	}

	GLuint shader = LoadProgram("D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.vert",
		"D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.frag");
	
	///////////////INITIALIZE VAO/////////////////
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mlVertex) * vertexCount, nullptr, GL_STATIC_DRAW);

	int vertexCounter = 0;
	for (int i = 0; i < model.meshes.size(); i++)
	{
		curMesh = &model.meshes.at(i);
		glBufferSubData(GL_ARRAY_BUFFER, vertexCounter, sizeof(mlVertex) * curMesh->vertices.size(), &curMesh->vertices.at(0));
		vertexCounter += curMesh->vertices.size();
	}


	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, nullptr, GL_STATIC_DRAW);

	int indexCounter = 0;
	for (int i = 0; i < model.meshes.size(); i++)
	{
		curMesh = &model.meshes.at(i);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexCounter, sizeof(unsigned int) * curMesh->indices.size(), &curMesh->indices.at(0));
		indexCounter += curMesh->vertices.size();
	}

	//////////////VERTEX ATTRIB POINTERS///////////////
	GLuint vPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) nullptr);

	GLuint vNormal = glGetAttribLocation(shader, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (sizeof(float) * 3));

	GLuint vUV = glGetAttribLocation(shader, "vUV");
	glEnableVertexAttribArray(vUV);
	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, sizeof(mlVertex), (GLvoid *) (sizeof(float) * 6));

	/////////////////OTHER INITIALIZATION///////////////////
	GLint perspectivePositionID = glGetUniformLocation(shader, "perspective");
	GLint transformPositionID = glGetUniformLocation(shader, "transform");

	glEnable(GL_DEPTH_TEST);

	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);


		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);

		modelMatrix = matrixTransform(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(thetaX, thetaY, thetaZ));
		transformMatrix = perspectiveMatrix * modelMatrix;

		thetaY -= 1.0f;
		thetaX -= 0.3f;
		thetaZ -= 0.04f;

		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &transformMatrix[0][0]);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();

	return EXIT_SUCCESS;
}

glm::mat4 matrixTransform(glm::vec3 const& translate, glm::vec3 const& scale, glm::vec3 const& rotate)
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), translate);
	glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return translateMatrix * rotateMatrix * scaleMatrix;
}