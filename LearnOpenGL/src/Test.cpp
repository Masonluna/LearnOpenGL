#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <iostream>
#include "VertexArray.h"


const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Initialize GLAD; NOTE: must make glfw context current before initializing GLAD. Order matters here.
	// Pass GLAD the function to load the OpenGL function pointers, glfwGetProcAddress defines the correct function based on OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	
	Shader ourShader("3.3.shader.coordsys.vs", "3.3.shader.coordsys.fs");

	GLCall(glEnable(GL_DEPTH_TEST));
	{
		// ========== DATA ==========

		glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};


		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};



		// Position of the origin, speed, and direction of the movement
		glm::vec3 position(0.0f, 0.0f, 0.0f);



		// ========== STATE ==========

		VertexArray va;
		VertexBuffer vb(vertices, sizeof(vertices));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		// TEXTURE
		// =========
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

		// ========== RENDERING ==========
		// Set up Render loop
		while (!glfwWindowShouldClose(window)) {
			// Time Logic
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			processInput(window);

			// rendering commands
			GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			// bind texture
			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
			GLCall(glActiveTexture(GL_TEXTURE1));
			GLCall(glBindTexture(GL_TEXTURE_2D, texture2));

			ourShader.use();

			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			ourShader.setMat4("projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			ourShader.setMat4("view", view);

			// render the box
			va.Bind();
			for (unsigned int i = 0; i < 10; i++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				ourShader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	GLCall(glViewport(0, 0, width, height));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	float camSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

