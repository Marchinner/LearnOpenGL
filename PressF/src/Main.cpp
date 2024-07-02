#include "../lib/Shader.h"

#include <GLFW/glfw3.h>

// Window Settings
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 400;
const char* TITLE = "Press F com cores";

// Function Prototypes
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	// Initialize GLFW
	glfwInit();
	// Set the window hint's (OpenGL v4.6)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	// Verify if the window was successfully created
	if (window != NULL)
	{
		std::cout << "SUCESSO::GLFW::CRIACAO_JANELA" << std::endl;
	}
	else
	{
		std::cout << "FALHA::GLFW::CRIACAO_JANELA" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the newly created window the current context
	glfwMakeContextCurrent(window);
	// Set the window resize callback
	glfwSetWindowSizeCallback(window, framebufferSizeCallback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FALHA::GLAD::INICIALIZACAO" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "SUCESSO::GLAD::INICIALIZACAO" << std::endl;
	}

	Shader myShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");

	// Define the rectangle vertices
	float vertices[] = {
		 -0.3f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 1.0 - 0
		 -0.1f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 1.1 - 1
		 -0.1f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 1.2 - 2
		 -0.3f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // 1.3 - 3
		 -0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // 2.0 - 4
		  0.1f, -0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // 2.1 - 5
		  0.1f,  0.1f, 0.0f, 0.0f, 1.0f, 0.0f, // 2.2 - 6
		 -0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 2.3 - 7
		 -0.1f,  0.3f, 0.0f, 0.0f, 0.0f, 1.0f, // 3.0 - 8
		  0.3f,  0.3f, 0.0f, 1.0f, 0.0f, 0.0f, // 3.1 - 9
		  0.3f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 3.2 - 10
		 -0.1f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f// 3.3 - 11
	};
	// Define the indices of the vertices that we want to draw
	unsigned int indices[] = {  // note that we start from 0!
		1, 0, 2,
		0, 3, 2,
		5, 4, 6,
		4, 7, 6,
		10, 9, 11,
		9, 8, 11
	};

	// Create the Vertex Array Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// Create the Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// Create the Element Buffer Objects
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object
	glBindVertexArray(VAO);

	// Bind the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the Vertex Buffer Object Data to send to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the Element Buffer Objects
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Set the Elements Buffer Data to send to GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set the Buffer Data Attributes Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// Enable the newly created Vertex Array Attribute
	glEnableVertexAttribArray(0);

	// Set the Buffer Data Attibutes Array for the color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Inputs
		processInput(window);

		// Set the clear window color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Render
		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

		// Swap buffers and poll the window events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
