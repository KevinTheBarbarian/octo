
#include "MatchRepeated.hh"

#include "Context.hh"

using namespace general;
using namespace std;

namespace parsing {
    MatchRepeated::MatchRepeated(const std::string& tag, const RuleRef& rule, int min, int max)
        : Matcher(tag), rule_(rule), min_(min), max_(max)
    {
    }

    ParseResult MatchRepeated::parse(const Context& context)
    {
        Context current = context;

        vector<ParseTree> subnodes;

        for(int i=0; i != max_; i++) {
            ParseResult partial = rule_.parse(current);

            if (! partial.isSuccess()) {
                if (i < min_) {
                    return ParseResult::fail(context);
                }
                break;
            }

            subnodes.push_back(partial.getParseTree());
            current = partial.getContext();
        }

        return ParseResult::parseBranch(current, getTag(), subnodes);
    }

    void MatchRepeated::print(std::ostream& out) const
    {
        out << rule_;

        if (min_ == 0 && max_ == 1) {
            out << "?";
        } else if (min_ == 0 && max_ == -1) {
            out << "*";
        } else if (min_ == 1 && max_ == -1) {
            out << "+";
        } else {
            out << "{" << min_ << "," << max_ << "}";
        }

        out << flush;
    }

    void MatchRepeated::scanGC(vector<HeapObject*>& queue)
    {
        rule_.scanGC(queue);
    }
}

