#ifndef CIRCLE_H
#define CIRCLE_H

class Circle {
public:
	Circle(double radius);  // 构造函数
	double area() const;   // 计算面积
	double circumference() const;  // 计算周长

private:
	double radius_;  // 圆的半径
	static constexpr double Pi = 3.14159265358979323846;  // 圆周率
};

#endif // CIRCLE_H