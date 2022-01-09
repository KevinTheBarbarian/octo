
#include "ObjectFactory.hh"
#include "SimpleSequence.hh"
#include "SimpleString.hh"

#include "string.h"

using namespace memory;

namespace types {
    ObjectFactory::ObjectFactory(OctoHeap& heap)
        : heap_(heap)
    {
    }

    OctoHeap& ObjectFactory::getHeap()
    {
        return heap_;
    }

    OctoSequence* ObjectFactory::makeSequence() const
    {
        OctoSequence* rv = new SimpleSequence;
        heap_.manageObject(rv);
        return rv;
    }

    OctoString* ObjectFactory::makeString(const char* str) const
    {
        return makeString(str, strlen(str));
    }

    OctoString* ObjectFactory::makeString(const char* str, size_t size) const
    {
        OctoString* rv = new SimpleString;
        rv->assign(str, size);

        heap_.manageObject(rv);
        return rv;
    }
}

