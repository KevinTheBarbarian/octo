
#include "MatchChoice.hh"

#include "Context.hh"

using namespace general;
using namespace std;

namespace parsing {
    MatchChoice::MatchChoice(const std::string& tag, const vector<RuleRef>& rules)
        : Matcher(tag), rules_(rules)
    {
        assert(rules.size());
    }

    ParseResult MatchChoice::parse(const Context& context)
    {
        if (context.isDebug()) cout << "MatchChoice: start, " << rules_.size() << " options\n";
        Context current = context;

        for(RuleRef& rr : rules_) {
            const Rule& r = rr.getRule(context);

            if (context.isDebug()) cout << "MatchChoice: trying rule: " << r << " at context " << context.getContextHint() << "\n";

            ParseResult result = r.parse(current);

            if (result.isSuccess()) {
                if (context.isDebug()) cout << "MatchChoice: success, new context '" << result.getContext().getContextHint() << "'\n";

                return ParseResult::parseBranch(result.getContext(),
                                                getTag(),
                                                {result.getParseTree()});
            } else {
                if (context.isDebug()) cout << "MatchChoice: rule: " << r << " failed at context " << context.getContextHint() << "\n";
            }

            current = context;
        }
        if (context.isDebug()) cout << "MatchChoice: failed, giving up\n";

        return ParseResult::fail(context);
    }

    void MatchChoice::print(std::ostream& out) const
    {
        out << "[";
        for(int i=0, e=rules_.size(); i != e; i++) {
            if (i) out << " | ";
            const RuleRef& rr = rules_[i];
            out << rr; //rules_[i];
        }
        out << "]" << flush;
    }

    void MatchChoice::scanGC(vector<HeapObject*>& queue)
    {
        for(int i=0, e=rules_.size(); i != e; i++) {
            rules_[i].scanGC(queue);
        }
    }
}

