
#ifndef __OCTO_STRING_HH__
#define __OCTO_STRING_HH__

#include "OctoObject.hh"

namespace types {
    class OctoString : public OctoObject {
    public:
        virtual void assign(const char* data, size_t size) = 0;

        // Compare to a null terminated string.
        virtual bool equalTo(const char* data) const = 0;

        virtual void toCppString(std::string& str) const = 0;
    };
}

#endif

