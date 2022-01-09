
#ifndef __HEAP_OBJECT_HH__
#define __HEAP_OBJECT_HH__

#include "common.hh"

namespace memory {
    class HeapObject {
    public:
        HeapObject();
        virtual ~HeapObject();

        void setGCMark(int8_t mark)
        {
            marked_ = mark;
        }

        int8_t getGCMark() const
        {
            return marked_;
        }

        virtual void scanGC(std::vector<HeapObject*>& queue) = 0;

    private:
        int8_t marked_;
    };
}

#endif

