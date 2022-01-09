
#ifndef __DIVIDE_HH__
#define __DIVIDE_HH__

#include "BinaryOperator.hh"

class Runtime;

class Divide : public BinaryOperator {
public:
    virtual PValue combine(Runtime& runtime, const PValue& a, const PValue& b);
};

#endif

