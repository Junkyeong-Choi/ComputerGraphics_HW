#include <GL/glew.h>
#include <GL/freeglut.h>
#include "gameObjects.h"

const int width = 768;
const int height = 432;
bool isFullScreen = false;

//const int width = 1920;
//const int height = 1080;
//bool isFullScreen = true;

int main(int argc, char* argv[]) {
	Game volleyball(argc, argv, width, height, isFullScreen);

	int delta;
	int lastFrame = glutGet(GLUT_ELAPSED_TIME);

	while (!volleyball.isExiting()) {
		int currentFrame = glutGet(GLUT_ELAPSED_TIME);
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;

		volleyball.update(delta);
		volleyball.render();
	}

	return 0;
}