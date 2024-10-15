#include <cstdlib>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "foo.h"
using namespace std;

using ::testing::Return;

class MockFoo : public FooInterface
{
public:
    MOCK_METHOD(std::string, getArbitraryString, ());
};

TEST(CASETEST, Case)
{

    string value = "Hello World!";

    // 第18行，声明一个MockFoo的对象：mockFoo
    MockFoo mockFoo;
    EXPECT_CALL(mockFoo, getArbitraryString()).Times(1).WillOnce(Return(value));

    // 是为MockFoo的getArbitraryString()方法定义一个期望行为，其中Times(1)的意思是运行一次，WillOnce(Return(value))的意思是第一次运行时把value作为getArbitraryString()方法的返回值
    string returnValue = mockFoo.getArbitraryString();
    cout << "Returned Value: " << returnValue << endl;
}


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}