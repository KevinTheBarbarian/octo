
#ifndef __VALUE_FACTORY_HH__
#define __VALUE_FACTORY_HH__

#include "Atom.hh"
#include "Integer.hh"
#include "Sequence.hh"

#include <memory>

class ValueFactory {
public:
    static PAtom atom(const char* text)
    {
        return PAtom(new Atom(text));
    }

    static PSequence sequence()
    {
        return PSequence(new Sequence);
    }

    static PSequence sequence(const std::initializer_list<PValue>& v)
    {
        return PSequence(new Sequence(v));
    }

    static PInteger integer(int64_t v)
    {
        return PInteger(new Integer(v));
    }

private:
    general::NotCopyable nocopy_;
};

#endif

