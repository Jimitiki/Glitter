// Local Headers
#include "glitter.hpp"
#include "ShaderHelpers.h"

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

	float positions[] = {
		-0.25f, -0.3f, 0.0f,
		0.25f, -0.3f, 0.0f,
		0.25f, 0.1f, 0.0f,
		0.0f, 0.4f, 0.0f,
		-0.25f, 0.1f, 0.0f
	};

	float colors[] = {
		0.2f, 0.2f, 0.2f,
		0.5f, 0.7f, 0.7f,
		0.6f, 0.6f, 0.6f,
		0.8f, 0.8f, 0.8f,
		0.9f, 1.0f, 0.2f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors),
		nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(colors), colors);


	GLuint shader = LoadProgram("D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.vert",
		"D:/Users/Chris/Documents/Visual Studio 2017/Projects/Glitter/Glitter/Shaders/basic.frag");

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);

		glBindVertexArray(vao);
		GLuint vPosition = glGetAttribLocation(shader, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) (nullptr));

		GLuint vColor = glGetAttribLocation(shader, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *) (sizeof(positions)));

		glm::mat4 t = matrixTransform(glm::vec3(0.3f, -0.3f, 0.0f), glm::vec3(0.9f, 0.9f, 0.0f));

		GLint transformPositionID = glGetUniformLocation(shader, "transform");
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &t[0][0]);

		GLint colorPositionID = glGetUniformLocation(shader, "colorMask");
		
		glm::mat4 c = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.6f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.6f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		glUniformMatrix4fv(colorPositionID, 1, GL_FALSE, &c[0][0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		t = matrixTransform(glm::vec3(0.1f, 0.1f, 0.0f), glm::vec3(1.2f, 1.2f, 0.0f));
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &t[0][0]);
		c = glm::mat4(
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
		);
		glUniformMatrix4fv(colorPositionID, 1, GL_FALSE, &c[0][0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

		t = matrixTransform(glm::vec3(-0.4f, 0.6f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f));
		glUniformMatrix4fv(transformPositionID, 1, GL_FALSE, &t[0][0]);
		c = glm::mat4(
			0.7f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.6f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.8f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		glUniformMatrix4fv(colorPositionID, 1, GL_FALSE, &c[0][0]);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}  glfwTerminate();

	return EXIT_SUCCESS;
}

glm::mat4 matrixTransform(glm::vec3 const& translate, glm::vec3 const& scale)
{
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), translate);
	return translateMatrix * scaleMatrix;
}
