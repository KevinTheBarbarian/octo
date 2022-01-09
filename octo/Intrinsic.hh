
#ifndef __INTRINSIC_HH__
#define __INTRINSIC_HH__

#include "Value.hh"

class Intrinsic : public Value {
public:
    Intrinsic()
        : Value(INTRINSIC)
    {
    }

    virtual PValue eval(Runtime& runtime, PValue& self);
};

typedef std::shared_ptr<Intrinsic> PIntrinsic;

#endif

