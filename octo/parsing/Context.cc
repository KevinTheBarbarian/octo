
#include "Context.hh"
#include "GrammarParser.hh"
#include "string_utils.hh"

using namespace general;

namespace parsing {
    Context::Context(GrammarParser* parser,
                     RuleRef        whitespace,
                     CharSlice      rest,
                     size_t         line,
                     size_t         offset,
                     bool           debug)
        : parser_    (parser),
          rest_      (rest),
          line_      (line),
          offset_    (offset),
          debug_     (debug)
    {
        whitespace_ = whitespace.getRule(*this);
    }

    CharSlice Context::getRest() const
    {
        return rest_;
    }

    size_t Context::getLine() const
    {
        return line_;
    }

    size_t Context::getOffset() const
    {
        return offset_;
    }

    Rule Context::getWhitespaceRule() const
    {
        return whitespace_;
    }

    Context Context::consume(size_t bytes) const
    {
        size_t newLine   = line_;
        size_t newOffset = offset_;

        updatePosition_(rest_.slice(0, bytes), newLine, newOffset);

        return Context(parser_, whitespace_, rest_.removeFront(bytes), newLine, newOffset, debug_);
    }

    void Context::updatePosition_(CharSlice data, size_t& newLine, size_t& newOffset)
    {
        size_t line = newLine, offset = newOffset;

        for(size_t i=0, e=data.size(); i != e; i++) {
            char ch = data[i];

            if (ch == '\n') {
                line++;
                offset = 0;
            } else {
                offset++;
            }
        }

        newLine   = line;
        newOffset = offset;
    }

    const Rule* Context::findRule(const std::string& name) const
    {
        return parser_->findRule(name);
    }

    std::string Context::getContextHint() const
    {
        CharSlice nearby = getRest();

        if (nearby.size() > 10) {
            nearby = nearby.slice(0, 10);
        }

        return filterPrintable(nearby);
    }

    bool Context::isDebug() const
    {
        return debug_;
    }
}

