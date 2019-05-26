#pragma once
#include "coordsystem.h"

struct Point {
	Point() = default;
	Point(double x, double y);

	double x;
	double y;
};

struct CoordSystem {
	int W; // weight of display
	int H; // height of display

	int gridX;
	int gridY;

	double hx;
	double hy;
	
	double shiftX; // shift on X
	double shiftY; // shift on Y

	double rangeX; // 
	double rangeY; // scale coefficient

	Point getDisplayCoordinate(Point p);
	Point getSceneCoordinate(Point p);

	void shift(int dx, int dy);
	void scale(double k);
	void resizeDisplay(int w, int h);
	void drawGrid();
	CoordSystem(int w, int h);
	CoordSystem() = default;
};

