
#ifndef __OCTO_SEQUENCE_HH__
#define __OCTO_SEQUENCE_HH__

#include "OctoObject.hh"

namespace types {
    class OctoSequence : public OctoObject {
    public:
        virtual int64_t size() const = 0;

        virtual const OctoObject* get(int64_t i) const = 0;

        virtual void append(OctoObject* obj) = 0;
    };
}

#endif

