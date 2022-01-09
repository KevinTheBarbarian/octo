
#include "HeapObject.hh"

namespace memory {
    HeapObject::HeapObject()
        : marked_(-1)
    {
    }

    HeapObject::~HeapObject()
    {
    }
}

