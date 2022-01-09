
#ifndef __ADD_HH__
#define __ADD_HH__

#include "BinaryOperator.hh"

class Runtime;

class Add : public BinaryOperator {
public:
    virtual PValue combine(Runtime& runtime, const PValue& a, const PValue& b);
};

#endif

