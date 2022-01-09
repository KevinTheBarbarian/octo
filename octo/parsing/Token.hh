
#ifndef __TOKEN_HH__
#define __TOKEN_HH__

#include "Matcher.hh"

namespace parsing {
    class Token : public Matcher {
    public:
        Token(const std::string& tag, const Rule& rule);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        static Context skipWhitespace_(const Rule& whitespace, const Context& context);

        Rule rule_;
    };
}

#endif

