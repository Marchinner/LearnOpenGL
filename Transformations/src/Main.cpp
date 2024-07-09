#define STB_IMAGE_IMPLEMENTATION
#include "../lib/Shader.h"
#include "../lib/stb_image.h"

#include <GLFW/glfw3.h>

// Window Settings
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 400;
const char* TITLE = "Triangulo com textura";

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

	Shader myShader("resources/shaders/vertexShader.glsl", "resources/shaders/fragmentShader.glsl");

	// Define the triangle vertices
	float vertices[] = {
		// positions         // colors         // texture
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f  // top 
	};

	// Texture Loading
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/textures/wall.jpg", &width, &height, &nrChannels, 0);

	// Generating the Texture
	GLuint texture;
	glGenTextures(1, &texture);
	// Binding the texture
	glBindTexture(GL_TEXTURE_2D, texture);
	// Texture configuration
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Generate
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Free the texture data
	stbi_image_free(data);


	// Create the Vertex Array Object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// Create the Vertex Buffer Object
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object
	glBindVertexArray(VAO);

	// Bind the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the Vertex Buffer Object Data to send to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set the Buffer Data Attributes Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// Enable the newly created Vertex Array Attribute
	glEnableVertexAttribArray(0);

	// Set the Buffer Data Attributes Array for the color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set the Buffer Data Attributes Array for the texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap buffers and poll the window events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
