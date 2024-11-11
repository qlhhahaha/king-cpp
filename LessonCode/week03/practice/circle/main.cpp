#include <iostream>
#include "circle.h"

int main() {
	std::cout << "Enter the radius of the circle: ";
	double radius;
	std::cin >> radius;

	Circle circle(radius);
	std::cout << "The area of the circle is: " << circle.area() << std::endl;
	std::cout << "The circumference of the circle is: " << circle.circumference() << std::endl;

	return 0;
}