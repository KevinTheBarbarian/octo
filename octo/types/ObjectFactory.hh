
#ifndef __OBJECT_FACTORY_HH__
#define __OBJECT_FACTORY_HH__

#include "OctoHeap.hh"
#include "OctoSequence.hh"
#include "OctoString.hh"

namespace types {
    // This class allocates objects on demand and uses a heap to manage them automatically.
    // However, since the objects are 'live' as soon as they are returned, be sure to add them to a
    // 'root' before the next allocation.

    class ObjectFactory : private general::NotCopyable {
    public:
        ObjectFactory(memory::OctoHeap& heap);

        memory::OctoHeap& getHeap();

        OctoSequence* makeSequence() const;
        OctoString*   makeString() const;
        OctoString*   makeString(const char* str) const;
        OctoString*   makeString(const char* str, size_t size) const;

    private:
        memory::OctoHeap& heap_;
    };
}

#endif

