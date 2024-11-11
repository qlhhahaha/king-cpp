#include <cmath>  // For std::fabs
#include "circle.h"  // Include the Circle class
#include "gtest/gtest.h"

// 测试 Circle 类的面积计算
TEST(CircleTest, Area) {
	Circle circle(5.0);
	double expected_area = 3.14159265358979323846 * 5.0 * 4.0;
	EXPECT_NEAR(circle.area(), expected_area, 0.0001);
}

// 测试 Circle 类的周长计算
TEST(CircleTest, Circumference) {
	Circle circle(5.0);
	double expected_circumference = 2 * 3.14159265358979323846 * 5.0;
	EXPECT_NEAR(circle.circumference(), expected_circumference, 0.0001);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}