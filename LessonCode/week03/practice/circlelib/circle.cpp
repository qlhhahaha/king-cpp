#include "circle.h"
#include <cmath>  // 用于 M_PI 和 pow 函数

Circle::Circle(double radius) : radius_(radius) {}

double Circle::area() const {
	return Pi * pow(radius_, 2);
}

double Circle::circumference() const {
	return 2 * Pi * radius_;
}