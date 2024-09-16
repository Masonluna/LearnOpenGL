#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"


glm::vec3 direction(0.11f, -0.09f, 0.0f);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
glm::vec3 updatePosition(glm::vec3& position);

const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{
	// ========== INIT ==========

	// Initialize GLFW and set the version to 3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Make sure the OpenGL version that we are using is the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertex Data
	// Let's Make a window!
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLAD; NOTE: must make glfw context current before initializing GLAD. Order matters here.
	// Pass GLAD the function to load the OpenGL function pointers, glfwGetProcAddress defines the correct function based on OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	
	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");

	float vertices[] = {
		// position				// color			// tex coord
		0.25f, 0.25f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.25f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.25f, -0.25f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.25f, 0.25f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Position of the origin, speed, and direction of the movement
	glm::vec3 position(0.0f, 0.0f, 0.0f);


	// Vertex Buffer Object Generation
	unsigned int VAO, EBO;

	VertexBuffer vb(vertices, sizeof(vertices));

	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &EBO));


	GLCall(glBindVertexArray(VAO)); // This is supposed to come first!!!
	// Bind Vertex Array Object first, then bind and set vertex buffers, then configure vertex attributes.

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	// Position attribute
	//                  LOC SIZ  TYPE     NORMALIZE?  SPACE BETWEEN ATT  OFFSET
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));

	// Color attribute
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	// Tex Coord Attribute
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(2));

	//Unbind array buffer now that glVertexAttribPointer registered VBO  as the vertex attribute's bound vertex buffer object
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	// load and create a texture
	// Texture 1
	// ---------
	unsigned int texture1;
	GLCall(glGenTextures(1, &texture1));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture1)); // Texture operations now apply to the object at texture

	// texture wrapping parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	// texture filtering parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// load image, create texture, and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);

	// Texture 2
	// ---------
	unsigned int texture2;
	GLCall(glGenTextures(1, &texture2));
	GLCall(glBindTexture(GL_TEXTURE_2D, texture2));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else 
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);
	
	
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	// Wireframe :)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ========== RENDERING ==========
	// Set up Render loop
	while (!glfwWindowShouldClose(window)) {
		// 1.) input
		processInput(window);

		// 2.) rendering commands
		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// bind texture
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture2));


		glm::mat4 transform = glm::mat4(1.0f);
		position = updatePosition(position);
		transform = glm::translate(transform, position);
		// transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// render the box
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		// 3.) check and call events and swap the buffers
		ourShader.use();
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		GLCall(glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform)));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Post window close cleanup
	GLCall(glDeleteVertexArrays(1, &VAO));
	//GLCall(glDeleteBuffers(1, &VBO));
	GLCall(glDeleteBuffers(1, &EBO));
	
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	GLCall(glViewport(0, 0, width, height));
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

glm::vec3 updatePosition(glm::vec3& position)
{
	float speed = 0.0012f;
	position += speed * direction;

	if (position.x > 1.0f - 0.25f || position.x < -1.0f + 0.25f) {
		direction.x = -direction.x;
	}
	if (position.y > 1.0f - 0.25f || position.y < -1.0f + 0.25f) {
		direction.y = -direction.y;
	}

	return position;
}