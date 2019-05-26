#include "pch.h"
#include "coordsystem.h"
#include <string>
#include <iostream>
#include <sstream>

Point::Point(double _x, double _y) {
	x = _x;
	y = _y;
}

CoordSystem::CoordSystem(int w, int h) {
	shiftX = 0;
	shiftY = 0;

	rangeX = 10;
	rangeY = 10;

	gridX = 8;
	gridY = 4;

	hx = rangeX / gridX;
	hy = rangeY / gridY;

	W = w;
	H = h;
}

void CoordSystem::resizeDisplay(int w, int h) {
	W = w; H = h;
}

void CoordSystem::shift(int dx, int dy) {
	shiftX += hx * dx;
	shiftY += hy * dy;
}

void CoordSystem::scale(double k) {
	rangeX *= k; hx = rangeX / gridX;
	rangeY *= k; hy = rangeY / gridY;
}

Point CoordSystem::getDisplayCoordinate(Point scene) {
	Point displ;

	displ.x = (scene.x - shiftX) * (float(W) / rangeX) + float(W) / 2;
	displ.y = H - (float(H) / 2 - (scene.y - shiftY)*(float(H) / rangeY));
	return displ;
}

Point CoordSystem::getSceneCoordinate(Point displ) {
	Point scene;

	scene.x = (displ.x - W / 2) / W * rangeX + shiftX;
	scene.y = (H / 2 - displ.y) / H * rangeY + shiftY;
	return scene;
}

void drawText(std::string text, int x, int y)
{
	glRasterPos2i(x, y);

	for (int i = 0; i < text.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
}

void drawValue(double value, int x, int y) {
	glRasterPos2i(x, y);
	std::stringstream line;
	line.precision(2);
	line << std::scientific;
	line << value;

	std::string text = "  ";
	line >> text;

	for (int i = 0; i < text.size(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}

}

void CoordSystem::drawGrid() {
	// draw Coordinate lines:
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);

	auto oo = getDisplayCoordinate(Point(0, 0));
	auto ox = getDisplayCoordinate(Point(1, 0));
	auto oy = getDisplayCoordinate(Point(0, 1));
	
	// x-axis:
	glVertex2d(ox.x * -W, ox.y);
	glVertex2d(ox.x * W, ox.y);


	glVertex2d(oy.x, oy.y * -H);
	glVertex2d(oy.x, oy.y * H);
	glEnd();

	// draw Grid lines
	glLineWidth(1);
	glBegin(GL_LINES);
	double stepX = (double)W / gridX;
	double midX = (double) W / 2;
	for (int i = 0; i <= (gridX / 2); i++) {
		glVertex2d(midX + stepX * i, 0);
		glVertex2d(midX + stepX * i, H);

		glVertex2d(midX - stepX * i, 0);
		glVertex2d(midX - stepX * i, H);
	}

	double stepY = (double)H / gridY;
	double midY = (double)H / 2;
	for (int i = 0; i <= (gridY / 2); i++) {
		glVertex2d(0, midY + stepY * i);
		glVertex2d(W, midY + stepY * i);

		glVertex2d(0, midY - stepY * i);
		glVertex2d(W, midY - stepY * i);
	}
	glEnd();
	
	// label of axis:
	drawText("x", W - 30, ox.y - 10);
	drawText("f(x)", oy.x + 15, H - 15);

	// value on grid:
	Point sceneMid = getSceneCoordinate(Point(double(W) / 2, double(H) / 2));
	double dx = rangeX / gridX;
	double dy = rangeY / gridY;

	drawValue(sceneMid.x, midX, midY - 13);
	/*drawText(std::to_string(sceneMid.x), midX, midY - 13);*/
	for (int i = 1; i <= (gridX / 2); i++) {
		drawValue(sceneMid.x + dx * i, midX + stepX * i, midY - 13);
		drawValue(sceneMid.x - dx * i, midX - stepX * i, midY - 13);
		/*drawText(std::to_string(sceneMid.x + dx * i), midX + stepX * i, midY - 13);*/
		//drawText(std::to_string(sceneMid.x - dx * i), midX - stepX * i, midY - 13);
	}

	//drawText(std::to_string(sceneMid.y), midX + 10, midY + 10);
	drawValue(sceneMid.y, midX + 10, midY + 10);
	for (int i = 1; i <= (gridY / 2); i++) {
		drawValue(sceneMid.y + dy * i, midX + 10, midY + 10 + stepY * i);
		drawValue(sceneMid.y - dy * i, midX + 10, midY + 10 - stepY * i);
		//drawText(std::to_string(sceneMid.y + dy * i), midX + 10, midY + 10 + stepY * i);
		//drawText(std::to_string(sceneMid.y - dy * i), midX + 10, midY + 10 - stepY * i);
	}
}