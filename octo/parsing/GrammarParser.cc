
#include "GrammarParser.hh"
#include "Context.hh"
#include "MatchChoice.hh"
#include "MatchSequence.hh"
#include "MatchString.hh"
#include "MatchRegex.hh"
#include "MatchRepeated.hh"
#include "ResultTransformer.hh"
#include "Token.hh"
#include "verify.hh"

using namespace general;
using namespace memory;
using namespace std;

namespace parsing {
    // GrammarParserFactory

    GrammarParserFactory::GrammarParserFactory(OctoHeap& heap)
        : heap_(heap)
    {
    }

    GrammarParserFactory::~GrammarParserFactory()
    {
        liveList_.clear();
    }

    void GrammarParserFactory::commitLiveList()
    {
        liveList_.clear();
    }

    void GrammarParserFactory::scanGC(std::vector<HeapObject*>& queue)
    {
        for(auto ptr : liveList_) {
            queue.push_back(ptr);
        }
    }

    MatchChoice* GrammarParserFactory::makeMatchChoice(const string& tag, const vector<RuleRef>& rules)
    {
        return manage(new MatchChoice(tag, rules));
    }

    MatchSequence* GrammarParserFactory::makeMatchSequence(const string& tag, const vector<RuleRef>& rules)
    {
        return manage(new MatchSequence(tag, rules));
    }

    MatchString* GrammarParserFactory::makeMatchString(const std::string& tag, const std::string& str)
    {
        return manage(new MatchString(tag, str));
    }

    MatchRegex* GrammarParserFactory::makeMatchRegex(const std::string& tag, const std::string& str)
    {
        return manage(new MatchRegex(tag, str));
    }

    MatchRepeated* GrammarParserFactory::makeMatchRepeated(const string& tag, const RuleRef& rule, int min, int max)
    {
        return manage(new MatchRepeated(tag, rule, min, max));
    }

    Token* GrammarParserFactory::makeToken(const string& tag, const Rule& rule)
    {
        return manage(new Token(tag, rule));
    }

    Rule* GrammarParserFactory::makeRule(Matcher* matcher)
    {
        return manage(new Rule(matcher));
    }

    ResultTransformer* GrammarParserFactory::makeResultTransformer(const std::string& tag,
                                                                   const RuleRef&     rule,
                                                                   ResultFunction*    rfn)
    {
        return manage(new ResultTransformer(tag, rule, rfn));
    }

    // GrammarParser

    GrammarParser::GrammarParser()
        : debug_  (false)
    {
        factory_ = new GrammarParserFactory(heap_);
        heap_.manageObject(factory_);
        heap_.addRoot(this);
    }

    GrammarParser::~GrammarParser()
    {
        factory_ = 0;
        ruleSet_.clear();

        heap_.garbageCollect();
        heap_.removeRoot(this);
    }

    ParseResult GrammarParser::parseSlice(RuleRef   startRule,
                                          RuleRef   whitespaceRule,
                                          CharSlice data)
    {
        Context context(this, whitespaceRule, data, 1, 0, debug_);
        return startRule.parse(context);
    }

    Rule GrammarParser::defineRule(const string& name, Matcher* matcher)
    {
        // There could also be a 'redefineRule' if we want to change rule definitions; it would
        // iterate over all rules and reset their name bindings, or something equivalent.

        heap_.manageObject(matcher);

        verify(ruleSet_.find(name) == ruleSet_.end(),
               string("Multiple definitions for rule: ") + name);

        Rule* rule = factory_->makeRule(matcher);
        ruleSet_[name] = rule;
        factory_->commitLiveList();

        static int collectAt = 20;

        if (collectAt-- <= 0) {
            collectAt = 20;
            heap_.garbageCollect();
        }

        return *rule;
    }

    const Rule* GrammarParser::findRule(const string& name) const
    {
        auto iter = ruleSet_.find(name);
        return (iter != ruleSet_.end()) ? (iter->second) : 0;
    }

    Rule GrammarParser::stringToken(const string& s)
    {
        return stringToken(s, s, s);
    }

    Rule GrammarParser::stringToken(const string& s, const string& tag)
    {
        return stringToken(s, s, tag);
    }

    Rule GrammarParser::stringToken(const string& name, const string& s, const string& tag)
    {
        return defineRule(name, factory_->makeToken("", factory_->makeMatchString(tag, s)));
    }

    Rule GrammarParser::regexToken(const string& name, const string& pattern, const string& tag)
    {
        return defineRule(name, factory_->makeToken("", factory_->makeMatchRegex(tag, pattern)));
    }

    Rule GrammarParser::matchRegex(const string& name, const string& pattern, const string& tag)
    {
        return defineRule(name, factory_->makeMatchRegex(tag, pattern));
    }

    Rule GrammarParser::matchChoice(const string& name, const RuleList& rules, const string& tag)
    {
        return defineRule(name, factory_->makeMatchChoice(tag, rules));
    }

    Rule GrammarParser::matchSequence(const string& name, const RuleList& rules, const string& tag)
    {
        return defineRule(name, factory_->makeMatchSequence(tag, rules));
    }

    Rule GrammarParser::zeroOrMore(const string& name, const RuleRef& r, const string& tag)
    {
        return repeated(name, r, 0, -1, tag);
    }

    Rule GrammarParser::oneOrMore(const string& name, const RuleRef& r, const string& tag)
    {
        return repeated(name, r, 1, -1, tag);
    }

    Rule GrammarParser::optional(const string& name, const RuleRef& r, const string& tag)
    {
        return repeated(name, r, 0, 1, tag);
    }

    Rule GrammarParser::repeated(const string& name, const RuleRef& r, int min, int max, const string& tag)
    {
        return defineRule(name, factory_->makeMatchRepeated(tag, r, min, max));
    }

    Rule GrammarParser::delimitedSequence(const string&  name,
                                          const RuleRef& r1,
                                          const RuleRef& delim,
                                          const string&  tag,
                                          int            min,
                                          int            max)
    {
        Rule opAndRhs = factory_->makeMatchSequence("", {delim, r1});
        Rule repeater = factory_->makeMatchRepeated("", opAndRhs, min, max);
        return defineRule(name, factory_->makeMatchSequence(tag, {r1, repeater}));
    }

    // Match a sequence represented by 'pattern', but transform the output.  One use of this is
    // to verify that an ident is not a reserved word, or is a specific word.
    Rule GrammarParser::transform(const string&   name,
                                  const RuleRef&  rule,
                                  ResultFunction* rfn,
                                  const string&   tag)
    {
        heap_.manageObject(rfn);
        return defineRule(name, factory_->makeResultTransformer(tag, rule, rfn));
    }

    void GrammarParser::setDebugMode(bool debug)
    {
        debug_ = debug;
    }

    void GrammarParser::scanGC(vector<HeapObject*>& queue)
    {
        queue.push_back(factory_);

        for(auto& entry : ruleSet_) {
            queue.push_back(entry.second);
        }
    }
}

