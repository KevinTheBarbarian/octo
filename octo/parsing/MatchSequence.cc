
#include "MatchSequence.hh"

#include "Context.hh"

using namespace general;
using namespace std;

namespace parsing {
    MatchSequence::MatchSequence(const std::string& tag, const vector<RuleRef>& rules)
        : Matcher(tag), rules_(rules)
    {
        assert(rules.size());
    }

    ParseResult MatchSequence::parse(const Context& context)
    {
        Context current = context;

        vector<ParseTree> subnodes;

        for(RuleRef& rr : rules_) {
            const Rule& r = rr.getRule(context);

            ParseResult partial = r.parse(current);

            if (! partial.isSuccess()) {
                // The 'partial' value should show the location near the error.
                return partial;
            }

            subnodes.push_back(partial.getParseTree());
            current = partial.getContext();
        }

        return ParseResult::parseBranch(current, getTag(), subnodes);
    }

    void MatchSequence::print(std::ostream& out) const
    {
        out << "[";
        for(int i=0, e=rules_.size(); i != e; i++) {
            if (i) out << ", ";
            const RuleRef& rr = rules_[i];
            out << rr; // rules_[i];
        }
        out << "]" << flush;
    }

    void MatchSequence::scanGC(vector<HeapObject*>& queue)
    {
        for(int i=0, e=rules_.size(); i != e; i++) {
            rules_[i].scanGC(queue);
        }
    }
}

