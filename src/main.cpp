#include <stdio.h>
#include <GLFW/glfw3.h>

bool loadFrame(const char* fileName, int* width, int* height, unsigned char** data);

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

	int frameWidth = 0;
	int frameHeight = 0;
	unsigned char* frameData;
	if (!loadFrame("D:\\Resources\\Video\\video.mp4", &frameWidth, &frameHeight, &frameData)) {
		printf("Could not load video frame\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	glClearColor(0.3, 0.5, 0.7, 1.0);

	//unsigned char* data = new unsigned char[256 * 256 * 3];
	//for (size_t y = 0; y < 255; y++)
	//{
	//	for (size_t x = 0; x < 255; x++)
	//	{
	//		data[y * 256 * 3 + x * 3 + 0] = 0;
	//		data[y * 256 * 3 + x * 3 + 1] = 255;
	//		data[y * 256 * 3 + x * 3 + 2] = 0;
	//	}
	//}
	//for (size_t y = 64; y < 192; y++)
	//{
	//	for (size_t x = 64; x < 192; x++)
	//	{
	//		data[y * 256 * 3 + x * 3 + 0] = 255;
	//		data[y * 256 * 3 + x * 3 + 1] = 255;
	//		data[y * 256 * 3 + x * 3 + 2] = 0;
	//	}
	//}

	GLuint textureID = -1;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, frameData);

	glBindTexture(GL_TEXTURE_2D, 0);

	//delete data;

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glDrawPixels(255, 255, GL_RGB, GL_UNSIGNED_BYTE, data);

		int windowWidth = 0;
		int windowHeight = 0;
		glfwGetFramebufferSize(window, &windowWidth, &windowHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, windowWidth, 0, windowHeight, -1, 1);

		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0); glVertex2i(0, 0);
		glTexCoord2d(1, 0); glVertex2i(frameWidth, 0);
		glTexCoord2d(1, 1); glVertex2i(frameWidth, frameHeight);
		glTexCoord2d(0, 1); glVertex2i(0, frameHeight);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		glfwSwapBuffers(window);

		glfwWaitEvents();
	}

	return 0;
}
