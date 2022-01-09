
#ifndef __OCTO_HEAP_HH__
#define __OCTO_HEAP_HH__

#include "HeapObject.hh"

#include "common.hh"

#include <memory>
#include <unordered_set>

namespace memory {
    class OctoHeap : private general::NotCopyable {
    public:
        OctoHeap();
        ~OctoHeap();

        void addRoot(HeapObject* obj);
        void removeRoot(HeapObject* obj);

        void manageObject(HeapObject* obj);
        void garbageCollect();

    private:
        void markRoots_();
        void mark_();
        void sweep_();

        std::unordered_set<HeapObject*> allocated_;
        std::unordered_set<HeapObject*> roots_;

        // The queue is used for garbage collection, but there is no point shrinking and
        // reallocating it each time, so here it is.

        std::vector<HeapObject*> queue_;
        int8_t currentMark_;
    };
}

#endif

