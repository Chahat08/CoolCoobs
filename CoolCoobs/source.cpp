#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>
#include <vector>

#include "Shader.h"
#include "VertexData.h"
#include "Constants.h"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int CURSOR_XPOS = INT_MIN;
int CURSOR_YPOS = INT_MIN;

int NUM_CUBES = 16;

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (CURSOR_XPOS == INT_MIN) {
		// set initial values
		CURSOR_XPOS = xpos;
		CURSOR_YPOS = ypos;
	}

	if (CURSOR_XPOS != xpos || CURSOR_YPOS != ypos)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void createModelMatrices(Shader &shader, std::vector<glm::vec3> positions, std::vector<glm::vec3> axes) {
	for (int i = 0; i < NUM_CUBES; ++i) {
		glm::mat4 model(1.0f);
		model = glm::translate(model, positions[i]);
		model = glm::rotate(model, (float)glfwGetTime(), axes[i]);

		shader.setUniformMatrix4float("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 12 * 6);
	}
}

void createViewMatrices(Shader& shader) {
	const float radius = 10.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	glm::mat4 view(1.0f);
	view = glm::lookAt(
		glm::vec3(camX, 0.0f, camZ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	shader.setUniformMatrix4float("view", view);
}

const char* vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec3 positionData;\n"
"layout(location = 1) in vec3 colorData;\n"
"uniform mat4 model, view, projection;\n"
"out vec3 fragmentColor;\n"
"void main() {\n"
"	gl_Position = projection * view * model * vec4(positionData, 1.0f);\n"
"	fragmentColor = colorData;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec3 fragmentColor;\n"
"out vec4 fragColor;\n"
"void main() {\n"
"	fragColor = vec4(fragmentColor, 1.0f);\n"
"}\0";

int WinMain() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DECORATED, NULL); // to remove border and titlebar

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	SCREEN_WIDTH = mode->width;
	SCREEN_HEIGHT = mode->height;

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Title", monitor, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	// creating shaders
	Shader shader(vertexShaderSource, fragmentShaderSource, false);

	// creating and sending vertex data
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositionsAndColors), &vertexPositionsAndColors, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_real_distribution<> distribution_xy(-5.0, 5.0);
	std::uniform_real_distribution<> distribution_z(-5.0, 5.0);
	std::uniform_real_distribution<> distribution_axes(0, 2 * PI);

	std::vector<glm::vec3> positions, axes;

	for (int i = 0; i < NUM_CUBES; ++i) {
		positions.push_back(glm::vec3(distribution_xy(engine), distribution_xy(engine), distribution_z(engine)));
		axes.push_back(glm::vec3(distribution_axes(engine), distribution_axes(engine), distribution_axes(engine)));
	}

	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0f));

	glm::mat4 projection(1.0f);
	projection = glm::perspective(
		glm::radians(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		0.1f,
		100.0f
	);



	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.0f, 0.027f, 0.212f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		shader.setUniformMatrix4float("projection", projection);
		createViewMatrices(shader);
		createModelMatrices(shader, positions, axes);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}