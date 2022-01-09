
#ifndef __MATCH_STRING_HH__
#define __MATCH_STRING_HH__

#include "Matcher.hh"

namespace parsing {
    class MatchString : public Matcher {
    public:
        MatchString(const std::string& tag, const std::string& str);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        std::string str_;
    };
}

#endif

