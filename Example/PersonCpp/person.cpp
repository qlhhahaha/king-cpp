#include "person.h"

Person::Person(const std::string name, const int age) : m_name(name), m_age(age) {}

std::string Person::getName() const {
    return m_name;
}

int Person::getAge() const {
    return m_age;
}
