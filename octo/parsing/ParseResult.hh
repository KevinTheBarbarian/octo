
#ifndef __PARSE_RESULT_HH__
#define __PARSE_RESULT_HH__

#include "Slice.hh"
#include "Context.hh"
#include "ParseTree.hh"

#include "common.hh"

namespace parsing {
    class ParseResult {
    public:
        ParseResult(const ParseTree& tree,
                    const Context&   context,
                    bool             success);

        ~ParseResult();

        static ParseResult fail(const Context& context);

        static ParseResult parseToken(const Context&     context,
                                      const std::string& tag,
                                      general::CharSlice token);

        static ParseResult parseBranch(const Context&         context,
                                       const std::string&     tag,
                                       std::vector<ParseTree> subNodes);

        ParseTree getParseTree() const;

        Context getContext() const;

        bool isSuccess() const;

    private:
        ParseTree tree_;
        Context   context_;
        bool      success_;
    };
}

#endif

