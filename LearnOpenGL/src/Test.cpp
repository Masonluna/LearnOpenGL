
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize GLFW and set the version to 3
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Make sure the OpenGL version that we are using is the core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Let's Make a window!
	GLFWwindow* window = glfwCreateWindow(640, 360, "LearnOpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Must be done before GLAD is initialized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	// Initialize GLAD; NOTE: must make glfw context current before initializing GLAD. Order matters here.
	// Pass GLAD the function to load the OpenGL function pointers, glfwGetProcAddress defines the correct function based on OS
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Tell OpenGL the window size
	// (0,0) == top-left pixel
	// (640, 360) == bottom-right pixel
	// (x == -1.0) -> leftmost pixel		(x == 1.0) -> rightmost pixel
	// (y == -1.0) -> top pixel				(y == 1.0) -> bottom pixel
	glViewport(0, 0, 640, 360);
	

	// Set up Render loop
	while (!glfwWindowShouldClose(window)) {
		// 1.) input
		processInput(window);

		// 2.) rendering commands
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 3.) check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Post window close cleanup
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}