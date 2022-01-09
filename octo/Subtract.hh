
#ifndef __SUBTRACT_HH__
#define __SUBTRACT_HH__

#include "BinaryOperator.hh"

class Runtime;

class Subtract : public BinaryOperator {
public:
    virtual PValue combine(Runtime& runtime, const PValue& a, const PValue& b);
};

#endif

