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

glm::mat4 matrixTransform(
	glm::vec3 const& translate,
	glm::vec3 const& scale
);


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

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	mlModel crayonModel;
	if (!LoadModel("D:/Users/Chris/Documents/CS455/Lab4/", "crayon.obj", crayonModel))
	{
		return -1;
	}

	GLuint shader = LoadProgram("D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.vert",
		"D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.frag");

	mlMesh crayonMesh = crayonModel.meshes[0];

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float positions[1584 * 3];
	for (int i = 0; i < crayonMesh.vertices.size(); i++)
	{
		int pIndex = i * 3;
		positions[pIndex] = crayonModel.meshes[0].vertices[i].position.x;
		positions[pIndex + 1] = crayonModel.meshes[0].vertices[i].position.y;
		positions[pIndex + 2] = crayonModel.meshes[0].vertices[i].position.z;
	}

	float UVs[1584 * 2];
	for (int i = 0; i < crayonMesh.vertices.size(); i++)
	{
		int uIndex = i * 2;
		UVs[uIndex] = crayonMesh.vertices[i].UV.x;
		UVs[uIndex + 1] = crayonMesh.vertices[i].UV.y;
	}

	//glBufferData(GL_ARRAY_BUFFER, sizeof(crayonMesh.vertices), (GLvoid *) crayonMesh.vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(UVs), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(UVs), UVs);


	GLuint vPosition = glGetAttribLocation(shader, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) (nullptr));

	GLuint vUV = glGetAttribLocation(shader, "vUV");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vUV, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid *) sizeof(positions));

	glm::mat4 p = glm::perspective(glm::radians(90.0f), (float) mWidth / (float) mHeight, 0.01f, 100.0f);
	GLint perspectivePositionID = glGetUniformLocation(shader, "perspective");

	glm::mat4 t = matrixTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	GLint transformPositionID = glGetUniformLocation(shader, "transform");

	//t = p * t;

	glm::vec4 v1 = p * t * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 v2 = p * t * glm::vec4(-0.2f, -0.2f, -0.2f, 1.0f);

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &t[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 1584);
		

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
