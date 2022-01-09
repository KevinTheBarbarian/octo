
#include "Token.hh"

#include "Context.hh"

using namespace general;
using namespace std;

namespace parsing {
    Token::Token(const std::string& tag, const Rule& rule)
        : Matcher(tag), rule_(rule)
    {
    }

    ParseResult Token::parse(const Context& context)
    {
        Rule whitespace = context.getWhitespaceRule();

        Context current = skipWhitespace_(whitespace, context);

        ParseResult p2 = rule_.parse(current);

        if (! p2.isSuccess()) {
            return p2;
        }

        current = skipWhitespace_(whitespace, p2.getContext());

        return ParseResult(p2.getParseTree(), current, true);
    }

    Context Token::skipWhitespace_(const Rule& whitespace, const Context& context)
    {
        ParseResult p3 = whitespace.parse(context);
        return p3.isSuccess() ? p3.getContext() : context;
    }

    void Token::print(std::ostream& out) const
    {
        out << "<" << rule_ << ">" << flush;
    }

    void Token::scanGC(vector<HeapObject*>& queue)
    {
        rule_.scanGC(queue);
    }
}

