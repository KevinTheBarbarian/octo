
#ifndef __HEAP_ROOT_HH__
#define __HEAP_ROOT_HH__

#include "OctoHeap.hh"
#include "HeapObject.hh"
#include "verify.hh"

namespace types {
    template<class T>
    class HeapRoot : public memory::HeapObject {
    public:
        HeapRoot(memory::OctoHeap& heap, T* obj = 0)
            : heap_ (heap),
              obj_  (obj)
        {
        }

        virtual void scanGC(std::vector<HeapObject*>& queue)
        {
            if (obj_) {
                queue.push_back(obj_);
            }
        }

        void set(T* obj)
        {
            obj_ = obj;
        }

        T* get() const
        {
            return obj_;
        }

        T* operator->()
        {
            general::verify(obj_, "null heap root");
            return get();
        }

        T& operator*()
        {
            general::verify(obj_, "null heap root");
            return *get();
        }

        const HeapRoot& operator=(const HeapRoot& other)
        {
            set(other.get());
            return *this;
        }

    private:
        memory::OctoHeap& heap_;
        T*                obj_;
    };
}

#endif

