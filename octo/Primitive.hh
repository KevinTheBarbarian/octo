
#ifndef __PRIMITIVE_HH__
#define __PRIMITIVE_HH__

#include "Value.hh"

class Primitive : public Value {
public:
    Primitive(BaseType bt)
        : Value(bt)
    {
    }

    virtual PValue eval(Runtime& runtime, PValue& self);
    virtual PValue apply(Runtime& runtime, std::shared_ptr<Sequence>& sequence);
};

typedef std::shared_ptr<Primitive> PPrimitive;

#endif

