
#ifndef __RULE_HH__
#define __RULE_HH__

#include "Printable.hh"
#include "HeapObject.hh"

#include <memory>

namespace parsing {
    class Context;
    class Matcher;
    class ParseResult;

    // This object has internal sharing.
    class Rule : public general::Printable, public memory::HeapObject {
    public:
        Rule();

        Rule(Matcher* matcher);

        ParseResult parse(const Context& data) const;

        virtual void print(std::ostream& ostr) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        Matcher* matcher_;
    };

    class RuleRef : public general::Printable, public memory::HeapObject {
    public:
        RuleRef(const char* name);
        RuleRef(const std::string& name);
        RuleRef(const Rule& rule);

        const Rule& getRule(const Context& context);

        ParseResult parse(const Context& data);

        virtual void print(std::ostream& ostr) const;

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        bool        haveRule_;
        std::string name_;
        Rule        rule_;
    };
}

#endif

