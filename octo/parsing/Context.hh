
#ifndef __CONTEXT_HH__
#define __CONTEXT_HH__

#include "common.hh"
#include "ParseTree.hh"
#include "Rule.hh"
#include "Slice.hh"

namespace parsing {
    class GrammarParser;

    class Context {
    public:
        Context(GrammarParser*     parser,
                RuleRef            whitespace,
                general::CharSlice rest,
                size_t             line,
                size_t             offset,
                bool               debug);

        general::CharSlice getRest() const;

        size_t getLine() const;
        size_t getOffset() const;

        Rule getWhitespaceRule() const;

        Context consume(size_t bytes) const;

        const Rule* findRule(const std::string& name) const;

        // gets a short block of the parse location for error messages or similar.
        std::string getContextHint() const;

        bool isDebug() const;

    private:
        static void updatePosition_(general::CharSlice bytes, size_t& line, size_t& offset);

        GrammarParser*     parser_;
        Rule               whitespace_;
        general::CharSlice rest_;
        size_t             line_;
        size_t             offset_;
        bool               debug_;
    };
}

#endif

