
#ifndef __MULTIPLY_HH__
#define __MULTIPLY_HH__

#include "BinaryOperator.hh"

class Runtime;

class Multiply : public BinaryOperator {
public:
    virtual PValue combine(Runtime& runtime, const PValue& a, const PValue& b);
};

#endif

