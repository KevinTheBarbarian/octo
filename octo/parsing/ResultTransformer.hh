
#ifndef __RESULT_TRANSFORMER_HH__
#define __RESULT_TRANSFORMER_HH__

#include "Matcher.hh"

namespace parsing {
    class ResultFunction : public memory::HeapObject, public general::Printable, private general::NotCopyable {
    public:
        virtual ~ResultFunction()
        {
        }

        virtual ParseResult operator()(ParseResult input) = 0;
    };

    class ResultTransformer : public Matcher {
    public:
        ResultTransformer(const std::string& tag,
                          const RuleRef&     rule,
                          ResultFunction*    rfn);

        virtual ParseResult parse(const Context& context);

        virtual void print(std::ostream& out) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        RuleRef         rule_;
        ResultFunction* rfn_;
    };
}

#endif

