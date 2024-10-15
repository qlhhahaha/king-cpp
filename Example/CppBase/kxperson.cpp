#include "kxperson.h"

KxPerson::KxPerson(const std::string& name)
	:m_name(name)
{

}

KxPerson::~KxPerson()
{
}

void KxPerson::showName() const
{
	std::cout << "name : " << m_name << std::endl;
}
