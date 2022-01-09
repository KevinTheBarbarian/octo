
#ifndef __MATCHER_HH__
#define __MATCHER_HH__

#include "ParseResult.hh"
#include "HeapObject.hh"

#include "common.hh"

#include <iostream>

namespace parsing {
    class Context;

    class Matcher : public general::Printable, public memory::HeapObject, private general::NotCopyable {
    public:
        Matcher(const std::string& tag);

        virtual ~Matcher();

        virtual ParseResult parse(const Context& context) = 0;

        virtual const std::string& getTag() const;

    private:
        const std::string tag_;
    };
}

#endif

