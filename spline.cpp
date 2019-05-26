#include "pch.h"
#include "spline.h"
#include "coordsystem.h"

extern CoordSystem CS;

// need test this scheme
void Spline::addPoint(const double x, const double f) {
	source.insert(xf(x, f));
	calculate();
}

void Spline::calculate() {
	//reset calculated array:
	calculated.clear();
	uncalculated.clear();

	// not enougt points to calculate spline
	if (source.size() < 4) {
		for (auto p: source) uncalculated.push_back(p);
		return;
	}

	auto it = source.begin();
	for (;;) {

		// check possibility calculate elem of spline
		auto temp = it;
		for (int i = 0; i < 4; i++) {
			if (temp == source.end()) {
				// if unpossibly: save uncalculated points
				while (it != source.end()) {
					uncalculated.push_back(*it);
					it++;
				}
				return;
			}
			else temp++;
		}

		// extract points and values
		const double x1 = it->first;
		const double f1 = it->second;
		it++;
		const double x2 = it->first;
		const double f2 = it->second;
		it++;
		const double x3 = it->first;
		const double f3 = it->second;
		it++;
		const double x4 = it->first;
		const double f4 = it->second;
		
		// coeficient for basis functions:
		const double length = x4 - x1;
		const double k1 = 0;
		const double k2 = (x2 - x1) / length;
		const double k3 = (x3 - x1) / length;
		const double k4 = 1;

		// define basis function:
		const double a1 = -1.0 / (k2*k3*k4);
		const double a2 = 1.0 / (k2 * (k2 - k3) * (k2 - k4));
		const double a3 = 1.0 / (k3 * (k3 - k2) * (k3 - k4));
		const double a4 = 1.0 / ((1 - k2)*(1 - k3));
		auto fi1 = [&](double ksi) {return  a1 * (ksi - k2)*(ksi - k3)*(ksi - k4); };
		auto fi2 = [&](double ksi) {return  a2 * (ksi)*(ksi - k3) * (ksi - k4); };
		auto fi3 = [&](double ksi) {return  a3 * (ksi)*(ksi - k2) * (ksi - k4); };
		auto fi4 = [&](double ksi) {return  a4 * (ksi)*(ksi - k2) * (ksi - k3); };

		// define function on elem
		auto elemf = [&](double x) {
			double ksi = (x - x1) / length;
			return f1 * fi1(ksi) + f2 * fi2(ksi) + f3 * fi3(ksi) + f4 * fi4(ksi);
		};

		const double step = length / frequency;
		for (int k = 0; k < frequency; k++) {
				double x = x1 + step * k;
			double f = elemf(x);

			calculated.push_back(xf(x,f));
		};
		calculated.push_back(xf(x4, f4));
	} 
}

void Spline::draw() {
	// draw source points:
	glPointSize(6.0);
	//glEnable(GL_SMOOTH);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	for (auto p : source) {
		Point displ = CS.getDisplayCoordinate(Point(p.first, p.second));
		glVertex2d(displ.x, displ.y);
	}
	glEnd();

	//// draw spline:
	glLineWidth(3);
	glBegin(GL_LINES);
	for (int i = 1; i < calculated.size(); i++) {
		Point displ1 = CS.getDisplayCoordinate(Point(calculated[i-1].first, calculated[i-1].second));
		Point displ2 = CS.getDisplayCoordinate(Point(calculated[i].first, calculated[i].second));

		glVertex2d(displ1.x, displ1.y);
		glVertex2d(displ2.x, displ2.y);
	}
	glEnd();

	// draw uncalculated points
	glColor3f(0, 1, 0);
	glLineWidth(3);
	glBegin(GL_LINES);
	for (int i = 1; i < uncalculated.size(); i++) {
		Point displ1 = CS.getDisplayCoordinate(Point(uncalculated[i - 1].first, uncalculated[i - 1].second));
		Point displ2 = CS.getDisplayCoordinate(Point(uncalculated[i].first, uncalculated[i].second));

		glVertex2d(displ1.x, displ1.y);
		glVertex2d(displ2.x, displ2.y);
	}
	glEnd();

}