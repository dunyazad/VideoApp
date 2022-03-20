#include <stdio.h>
#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
	GLFWwindow* window;

	if (!glfwInit()) {
		printf("Could not init GLFW\n");
		return 1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
	if (!window) {
		printf("Could not open window\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	glClearColor(0.3, 0.5, 0.7, 1.0);

	unsigned char* data = new unsigned char[255 * 255 * 3];
	for (size_t y = 0; y < 255; y++)
	{
		for (size_t x = 0; x < 255; x++)
		{
			data[y * 255 * 3 + x * 3 + 0] = x;
			data[y * 255 * 3 + x * 3 + 1] = y;
			data[y * 255 * 3 + x * 3 + 2] = (x + y) / 2;
		}
	}

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(255, 255, GL_RGB, GL_UNSIGNED_BYTE, data);

		glfwSwapBuffers(window);

		glfwWaitEvents();
	}

	return 0;
}
