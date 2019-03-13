#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/geometric.hpp>
#include "ballObject.h"

void setCamera(bool ballCameraMode, BallObject ball) {
	if (ballCameraMode) {
		const float width = 192.0 * 3 / 4;
		const float height = 108.0 * 3 / 4;
		glm::vec2 pos = ball.getPosition();
		float radius = ball.getRadius();
		pos.x += radius;
		pos.y += radius;
		float x, y;

		if (pos.x < width / 2)
			x = 0;
		else if (pos.x > 192.0 - width / 2)
			x = 192.0 - width;
		else
			x = pos.x - width / 2;

		if (pos.y < height / 2)
			y = 0;
		else if (pos.y > 108.0 - height / 2)
			y = 108.0 - height;
		else
			y = pos.y - height / 2;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, width, 0.0, height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-x, -y, 0.0);
		gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, 192.0, 0.0, 108.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}