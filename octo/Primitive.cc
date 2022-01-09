
#include "Primitive.hh"

#include "Sequence.hh"

using namespace general;

PValue Primitive::eval(Runtime& runtime, PValue& self)
{
    return self;
}

PValue Primitive::apply(Runtime& runtime, PSequence& sequence)
{
    verify(false, "apply(...) called for primitive");
    return PValue();
}

