
#ifndef __SIMPLE_STRING_HH__
#define __SIMPLE_STRING_HH__

#include "OctoString.hh"

namespace types {
    class SimpleString : public OctoString {
    public:
        virtual void assign(const char* data, size_t size);

        // Compare to a null terminated string.
        virtual bool equalTo(const char* data) const;

        virtual void toCppString(std::string& str) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);
        virtual void printObject(std::ostream& output) const;

    private:
        std::string data_;
    };
}

#endif

