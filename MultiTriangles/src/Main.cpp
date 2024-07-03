#include "../lib/Shader.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Window Settings
const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 400;
const char* TITLE = "Varias Formas Geometricas (EBO)";

// Functions Prototypes
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// Init GLFW
	glfwInit();

	// Configure the window that will be created
	// Set OpenGL Context version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Set OpenGL Core Profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
	// Check if the window was successfully created
	if (window != NULL)
	{
		std::cout << "SUCESSO::GLFW::JANELA_CRIADA" << std::endl;
	}
	else
	{
		std::cout << "FALHA::GLFW::CRIACAO_JANELA_FALHOU" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make this window the current context for OpenGL
	glfwMakeContextCurrent(window);

	// Set the window resize callback function
	glfwSetWindowSizeCallback(window, framebufferSizeCallback);

	// Init Glad
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "SUCESSO::GLAD::GLAD_INICIALIZADO" << std::endl;
	}
	else
	{
		std::cout << "FALHA::GLAD::GLAD_NAO_INICIALIZADO" << std::endl;
		return -1;
	}

	// Compile Shaders
	Shader myShader("resources/shaders/vertexShader.glsl", "resources/shaders/fragmentShader.glsl");

	// Set the vertices that we want to draw
	float vertices[] = {
		-0.8f, 0.4f, 0.0f,
		-0.8f,-0.4f, 0.0f,
		-0.2f,-0.4f, 0.0f,
		-0.2f, 0.4f, 0.0f,
		 0.2f,-0.4f, 0.0f,
		 0.8f,-0.4f, 0.0f,
		 0.5f, 0.4f, 0.0f
	};

	// Set the order that we want to draw
	unsigned int indices[] = {
		2, 1, 3,
		1, 0, 3,
		4, 5, 6
	};

	// Create the VAO, EBO and VBO
	GLuint VAO, EBO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the VBO data, attributes and enable
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the EBO with de defined VBO's
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Set the EBO data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Sets the attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Enable the attribute array
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->Build();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	//ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	bool show = true;
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Process all the inputs
		processInput(window);

		// Set the OpenGL clear color
		glClearColor(0.75f, 0.75f, 0.5f, 1.0f);
		// Clear the color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rendering
		myShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap the buffers and poll the events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwDestroyWindow(window);
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