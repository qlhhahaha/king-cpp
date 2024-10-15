#ifndef FOOINTERFACE_H_
#define FOOINTERFACE_H_

#include <string>


class FooInterface {
public:
        virtual ~FooInterface() {}

public:
        virtual std::string getArbitraryString() = 0;
};


#endif // FOOINTERFACE_H_
