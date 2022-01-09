
#ifndef __INTEGER_HH__
#define __INTEGER_HH__

#include "Primitive.hh"

class Integer : public Primitive {
public:
    Integer(int64_t x)
        : Primitive(INTEGER), x_(x)
    {
    }

    int64_t getInteger()
    {
        return x_;
    }

private:
    int64_t x_;
};

typedef std::shared_ptr<Integer> PInteger;

#endif

