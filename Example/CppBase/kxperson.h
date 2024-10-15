#ifndef __KX_PERSON_H__
#define __KX_PERSON_H__

#include <iostream>

class KxPerson
{
public:
	KxPerson() = default;
	KxPerson(const std::string& name);
	~KxPerson();
	virtual void showName() const;
private:
	std::string m_name;
};



#endif