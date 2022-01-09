
#ifndef __MATCH_REPEATED_HH__
#define __MATCH_REPEATED_HH__

#include "Matcher.hh"

namespace parsing {
    class MatchRepeated : public Matcher {
    public:
        MatchRepeated(const std::string& tag, const RuleRef& rule, int min, int max);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        RuleRef rule_;
        int     min_;
        int     max_;
    };
}

#endif

