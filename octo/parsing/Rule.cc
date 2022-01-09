
#include "Rule.hh"
#include "Matcher.hh"
#include "verify.hh"

#include <memory>

using namespace general;
using namespace std;

namespace parsing {
    Rule::Rule()
        : matcher_(0)
    {
    }

    Rule::Rule(Matcher* matcher)
        : matcher_(matcher)
    {
    }

    ParseResult Rule::parse(const Context& context) const
    {
        verify(matcher_, "Null matcher");
        return matcher_->parse(context);
    }

    void Rule::print(std::ostream& out) const
    {
        if (matcher_) {
            matcher_->print(out);
        } else {
            out << "null";
        }
        out << flush;
    }

    void Rule::scanGC(vector<HeapObject*>& queue)
    {
        queue.push_back(matcher_);
    }

    // RuleRef

    RuleRef::RuleRef(const char* name)
        : haveRule_(false),
          name_    (name)
    {
    }

    RuleRef::RuleRef(const std::string& name)
        : haveRule_(false),
          name_    (name)
    {
    }

    RuleRef::RuleRef(const Rule& rule)
        : haveRule_(true),
          rule_    (rule)
    {
    }

    const Rule& RuleRef::getRule(const Context& context)
    {
        if (! haveRule_) {
            const Rule* r = context.findRule(name_);
            verify(r, string("Parse rule [") + name_ + "] not defined.");

            rule_     = *r;
            haveRule_ = true;
        }
        return rule_;
    }

    ParseResult RuleRef::parse(const Context& context)
    {
        if (context.isDebug()) cout << "RuleRef, parsing [" << *this << "]...\n";
        return getRule(context).parse(context);
    }

    void RuleRef::print(std::ostream& out) const
    {
        // If a name exists, use that. This should simplify output as well as hopefully prevent
        // infinite recursion.
        if (name_.empty()) {
            out << "ref(rule=" << rule_ << ")";
        } else {
            out << "ref(name=" << name_ << ")";
        }
        out << flush;
    }

    void RuleRef::scanGC(vector<HeapObject*>& queue)
    {
        rule_.scanGC(queue);
    }
}

