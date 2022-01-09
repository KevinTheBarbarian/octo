
#ifndef __MATCH_CHOICE_HH__
#define __MATCH_CHOICE_HH__

#include "Matcher.hh"

namespace parsing {
    class MatchChoice : public Matcher {
    public:
        MatchChoice(const std::string& tag, const std::vector<RuleRef>& rules);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        std::vector<RuleRef> rules_;
    };
}

#endif

