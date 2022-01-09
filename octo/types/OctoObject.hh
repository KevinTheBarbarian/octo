
#ifndef __OCTO_OBJECT_HH__
#define __OCTO_OBJECT_HH__

#include "HeapObject.hh"
#include <iostream>

namespace types {
    class OctoObject : public memory::HeapObject {
    public:
        virtual void printObject(std::ostream& output) const = 0;
    };

    static inline std::ostream& operator<<(std::ostream& output, const OctoObject& obj)
    {
        obj.printObject(output);
        return output;
    }

    static inline std::ostream& operator<<(std::ostream& output, const OctoObject* obj)
    {
        if (obj) {
            output << *obj;
        } else {
            output << "<null>";
        }
        return output;
    }
}

#endif

