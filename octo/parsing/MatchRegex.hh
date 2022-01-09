
#ifndef __MATCH_REGEX_HH__
#define __MATCH_REGEX_HH__

#include "Matcher.hh"
#include "PcrePattern.hh"

namespace parsing {
    class MatchRegex : public Matcher {
    public:
        MatchRegex(const std::string& tag, const std::string& str);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        std::string str_;
        PcrePattern pattern_;
    };
}

#endif

