
#ifndef __BINARY_OPERATOR_HH__
#define __BINARY_OPERATOR_HH__

#include "Intrinsic.hh"

class Runtime;

class BinaryOperator : public Intrinsic {
public:
    virtual PValue apply(Runtime& runtime, std::shared_ptr<Sequence>& sequence);
    virtual PValue combine(Runtime& runtime, const PValue& a, const PValue& b) = 0;
};

#endif

