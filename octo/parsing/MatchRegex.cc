
#include "MatchRegex.hh"

#include "Context.hh"

using namespace general;

namespace parsing {
    namespace {
        int pcreCompileOptions()
        {
            return PCRE_ANCHORED;
        }
    }

    MatchRegex::MatchRegex(const std::string& tag, const std::string& str)
        : Matcher  (tag),
          str_     (str),
          pattern_ (str, pcreCompileOptions())
    {
    }

    ParseResult MatchRegex::parse(const Context& context)
    {
        CharSlice rest = context.getRest();
        int matchOptions = PCRE_ANCHORED;

        CharSlice matched;

        if (pattern_.matchAnchored(rest, matchOptions, matched)) {
            return ParseResult::parseToken(context.consume(matched.size()), getTag(), matched);
        }

        return ParseResult::fail(context);
    }

    void MatchRegex::print(std::ostream& out) const
    {
        out << "rx(/" << str_ << "/)" << flush;
    }

    void MatchRegex::scanGC(vector<HeapObject*>& queue)
    {
    }
}

