#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
public:
    Person(const std::string name, const int age);
    std::string getName() const;
    int getAge() const;

private:
    std::string m_name;
    int m_age;
};

#endif
