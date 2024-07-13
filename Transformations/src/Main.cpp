#define STB_IMAGE_IMPLEMENTATION
#include "../lib/Shader.h"
#include "../lib/stb_image.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
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
	float vertices[]{
		// positions         // colors         // texture
		-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,-0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
		 0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.5f, 1.0f
	};

	unsigned int index[]{
		0, 1, 3,
		3, 2, 1,
		1, 4, 2,
		2, 3, 4,
		4, 3, 0,
		0, 1, 4
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
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object
	glBindVertexArray(VAO);

	// Bind the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the Vertex Buffer Object Data to send to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.Fonts->Build();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// ImGui
	float clearColor[3]{
		0.5f, 0.5f, 0.5f
	};

	float viewPosition[3]{
		0.0f, 0.0f, -3.0f
	};

	float color[4]{
		0.5f, 0.5f, 0.5f, 1.0f
	};

	float rotationAxis[3]{
		0.0f, 1.0f, 0.0f
	};

	float rotationAngle{ 0.0f };

	float fovy = 0.0f;

	glEnable(GL_DEPTH_TEST);
	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Process Inputs
		processInput(window);

		// Set the clear window color
		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rotationAngle = glfwGetTime() * 100.0f;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(rotationAxis[0], rotationAxis[1], rotationAxis[2]));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(viewPosition[0], viewPosition[1], viewPosition[2]));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);



		// Render
		myShader.use();
		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_INT, 0);

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(450, 180));
		ImGui::Begin("Ferramentas", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		ImGui::ColorEdit3("Cor de Fundo", clearColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);
		ImGui::DragFloat3("Posicao da View", viewPosition, 0.01f, -100.0f, 100.0f, "%.2f");
		ImGui::ColorEdit4("Cor do Objeto", color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);
		ImGui::DragFloat3("Eixo de Rotacao", rotationAxis, 0.01f, -1.0f, 1.0f, "%.2f");
		ImGui::SliderAngle("Angulo de Rotacao", &rotationAngle, -360.0f, 360.0F);
		ImGui::DragFloat("Fov", &fovy, 0.001f, -1.0f, 1.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
