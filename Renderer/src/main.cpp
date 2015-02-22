#include "WindowManager.h"
#include "GLUtil.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

float vertices[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

struct AppData {
	GLFWwindow* window;

	GLuint shaderID;
	GLint mvpID, modeID;
	glm::mat4 m, v, p;

	GLuint vao, vbo;

	Texture sdf;

	AppData(GLFWwindow* window);
	void updateMatrices();
	void update(float dt);
	void render();
};

static void resizeCallback(GLFWwindow *window, int width, int height) {
	AppData* app = (AppData*)glfwGetWindowUserPointer(window);
	printf("Setting viewport: %dx%d\n", width, height);
	glViewport(0, 0, width, height);
	app->p = glm::perspective(45.0f, float(width) / float(height), 0.1f, 100.0f);
	app->updateMatrices();
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	AppData* app = (AppData*)glfwGetWindowUserPointer(window);
	if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) glUniform1i(app->modeID, key - GLFW_KEY_1 + 1);
	if (key == GLFW_KEY_N) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	if (key == GLFW_KEY_L) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

int main(int argc, char* argv[]) {
	Window::setContextVersion(4, 0, false, false);
	GLFWwindow* window = Window::create("Test", 800, 600, FLAG_RESIZEABLE | FLAG_CENTERED | FLAG_VISIBLE);
	if (!window) {
		fprintf(stderr, "Failed to create window\n");
		return 1;
	}

	if (!Window::createContext(window)) {
		fprintf(stderr, "GLEW failed to initialize \n");
		return 1;
	}

	//glfwSwapInterval(0);
	glfwSetFramebufferSizeCallback(window, resizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	AppData app(window);

	double prev, curr = glfwGetTime();
	int frames = 0;
	double time = 0.0;

	while (!glfwWindowShouldClose(window)) {
		prev = curr;
		curr = glfwGetTime();

		frames++;
		time += curr - prev;
		if (time > 5.0) {
			printf("%.2ffps\n", (double(frames) / 5.0));
			frames = 0;
			time -= 5.0;
		}

		app.update(curr - prev);
		app.render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

AppData::AppData(GLFWwindow* window) : window(window), sdf("font.png", GL_LINEAR, GL_LINEAR) {
	//Add a pointer to this struct to the window
	glfwSetWindowUserPointer(window, this);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Initialize shaders
	GLuint vert = GLUtil::loadShader(GL_VERTEX_SHADER, "test.vert");
	GLuint frag = GLUtil::loadShader(GL_FRAGMENT_SHADER, "test.frag");
	shaderID = GLUtil::createProgram({ vert, frag });
	glUseProgram(shaderID);
	mvpID = glGetUniformLocation(shaderID, "mvp");
	modeID = glGetUniformLocation(shaderID, "mode");
	glUniform1i(modeID, 1);
	glDeleteShader(frag);
	glDeleteShader(vert);

	//Initialize matrices
	m = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
	v = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	resizeCallback(window, width, height);

	glClearColor(0.0f, 0.15f, 0.35f, 0.0f);
}

void AppData::updateMatrices() {
	glm::mat4 mvp = p * v * m;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
}

void AppData::update(float dt) {
	glm::vec3 translation(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_W))
		translation.y -= dt;
	if (glfwGetKey(window, GLFW_KEY_S))
		translation.y += dt;
	if (glfwGetKey(window, GLFW_KEY_A))
		translation.x += dt;
	if (glfwGetKey(window, GLFW_KEY_D))
		translation.x -= dt;
	if (glfwGetKey(window, GLFW_KEY_E))
		translation.z += dt;
	if (glfwGetKey(window, GLFW_KEY_Q))
		translation.z -= dt;
	if (translation.x || translation.y || translation.z) {
		m = glm::translate(m, translation);
		updateMatrices();
	}
}

void AppData::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices) / 2);
}