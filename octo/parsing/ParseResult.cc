
#include "ParseResult.hh"

using namespace general;
using namespace std;

namespace parsing {
    ParseResult::ParseResult(const ParseTree& tree,
                             const Context&   context,
                             bool             success)
        : tree_   (tree),
          context_(context),
          success_(success)
    {
    }

    ParseResult::~ParseResult()
    {
    }

    ParseResult ParseResult::fail(const Context& context)
    {
        return ParseResult(ParseTree(), context, false);
    }

    ParseResult ParseResult::parseToken(const Context& context, const std::string& tag, CharSlice token)
    {
        return ParseResult(ParseTree(tag, token), context, true);
    }

    ParseResult ParseResult::parseBranch(const Context& context, const std::string& tag, vector<ParseTree> subNodes)
    {
        return ParseResult(ParseTree(tag, subNodes), context, true);
    }

    bool ParseResult::isSuccess() const
    {
        return success_;
    }

    Context ParseResult::getContext() const
    {
        return context_;
    }

    ParseTree ParseResult::getParseTree() const
    {
        return tree_;
    }
}

