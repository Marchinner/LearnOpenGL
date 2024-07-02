#include "Shader.h"

#include <GLFW/glfw3.h>

// Window Settings
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 400;
const char* TITLE = "Primeiro Triangulo!!";

// Function Prototypes
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Initialize GLFW
	glfwInit();
	// Configure the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	// Verify if the window has successfully created
	if (window == NULL)
	{
		std::cout << "FALHA::GLFW::CRIACAO_DA_JANELA" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make this window the current context
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FALHA::GLAD::INICIALIZACAO_GLAD" << std::endl;
		return -1;
	}

	// Creates the Shader Program
	Shader myShader("vertexShader.glsl", "fragmentShader.glsl");

	// Define the vertices of the triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	// left		vertice
		 0.5f, -0.5f, 0.0f,	// right	vertice
		 0.0f,  0.5f, 0.0f	// top		vertice
	};

	// Create the Vertex Array Object (VAO)
	GLuint VAO;	// VAO unique ID
	glGenVertexArrays(1, &VAO);
	// Create the Vertex Buffer Object (VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Bind the VAO
	glBindVertexArray(VAO);
	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set the VBO data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Set the Vertex Attribute Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute Array
	glEnableVertexAttribArray(0);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Input
		processInput(window);

		// Clear the window
		glClearColor(0.2f, 0.5f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Rendering
		myShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Displaying and Poll Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}