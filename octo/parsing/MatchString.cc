
#include "MatchString.hh"
#include "Context.hh"

#include "string_utils.hh"

using namespace general;
using namespace std;

namespace parsing {
    MatchString::MatchString(const std::string& tag, const string& str)
        : Matcher(tag), str_(str)
    {
    }

    ParseResult MatchString::parse(const Context& context)
    {
        if (context.isDebug()) cout << "Checking for string match '" << str_ << "'\n";
        CharSlice rest = context.getRest();

        if (hasPrefix(rest, CharSlice(str_))) {
            if (context.isDebug()) cout << "Success\n";
            return ParseResult::parseToken(context.consume(str_.size()), getTag(), CharSlice(str_));
        }

        if (context.isDebug()) cout << "No match\n";
        return ParseResult::fail(context);
    }

    void MatchString::print(ostream& out) const
    {
        out << "str('" << str_ << "')" << flush;
    }

    void MatchString::scanGC(vector<HeapObject*>& queue)
    {
    }
}

