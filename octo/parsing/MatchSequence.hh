
#ifndef __MATCH_SEQUENCE_HH__
#define __MATCH_SEQUENCE_HH__

#include "Matcher.hh"

namespace parsing {
    class MatchSequence : public Matcher {
    public:
        MatchSequence(const std::string& tag, const std::vector<RuleRef>& rules);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        std::vector<RuleRef> rules_;
    };
}

#endif

