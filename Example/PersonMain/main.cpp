#include "person.h"
#include <iostream>

int main()
{
    Person p("jack", 1);
    std::cout << p.getAge() << std::endl;
    return 0;
}