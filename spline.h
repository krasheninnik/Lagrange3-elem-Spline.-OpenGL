#pragma once
#include <vector>
#include <set>

class Spline {
	using xf = std::pair<double, double>;
public:
	void addPoint(const double x, const double f);
	void calculate();
	void draw();

private:
	std::set<xf> source; // for input points
	std::vector<xf> calculated; // for value of spline
	std::vector<xf> uncalculated; // for not used source point
	const int frequency = 200;
	const int pointForElem = 4;
};