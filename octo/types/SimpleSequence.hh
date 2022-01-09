
#ifndef __SIMPLE_SEQUENCE_HH__
#define __SIMPLE_SEQUENCE_HH__

#include "OctoSequence.hh"

namespace types {
    class SimpleSequence : public OctoSequence {
    public:
        virtual int64_t           size() const;
        virtual const OctoObject* get(int64_t i) const;

        virtual void append(OctoObject* obj);

        virtual void scanGC(std::vector<HeapObject*>& queue);
        virtual void printObject(std::ostream& output) const;

    private:
        std::vector<OctoObject*> data_;
    };
}

#endif

