
#ifndef __GRAMMAR_PARSER_HH__
#define __GRAMMAR_PARSER_HH__

#include "HeapObject.hh"
#include "OctoHeap.hh"
#include "Rule.hh"
#include "ResultTransformer.hh"
#include "Slice.hh"

#include <map>

namespace parsing {
    class ParseResult;

    class GrammarParserFactory : public memory::HeapObject, private general::NotCopyable {
    public:
        GrammarParserFactory(memory::OctoHeap& heap);
        virtual ~GrammarParserFactory();

        class MatchRepeated* makeMatchRepeated(const std::string& tag, const RuleRef& rule, int min, int max);
        class MatchChoice*   makeMatchChoice(const std::string& tag, const std::vector<RuleRef>& rules);
        class MatchSequence* makeMatchSequence(const std::string& tag, const std::vector<RuleRef>& rules);
        class MatchString*   makeMatchString(const std::string& tag, const std::string& str);
        class MatchRegex*    makeMatchRegex(const std::string& tag, const std::string& str);
        class Token*         makeToken(const std::string& tag, const Rule& rule);
        Rule*                makeRule(Matcher* matcher);
        ResultTransformer*   makeResultTransformer(const std::string& tag,
                                                   const RuleRef&     rule,
                                                   ResultFunction*    rfn);

        void commitLiveList();

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        template<class T>
        T* manage(T* obj)
        {
            heap_.manageObject(obj);
            liveList_.push_back(obj);
            return obj;
        }

        std::vector<memory::HeapObject*> liveList_;
        memory::OctoHeap&                heap_;
    };

    class GrammarParser : public memory::HeapObject, private general::NotCopyable {
        typedef std::initializer_list<RuleRef> RuleList;

    public:
        GrammarParser();
        virtual ~GrammarParser();

        ParseResult parseSlice(RuleRef            startRule,
                               RuleRef            whitespaceRule,
                               general::CharSlice data);

        Rule defineRule(const std::string& name, Matcher* rule);

        Rule stringToken(const std::string& str);
        Rule stringToken(const std::string& str, const std::string& tag);
        Rule stringToken(const std::string& name, const std::string& str, const std::string& tag);

        Rule matchRegex(const std::string& name, const std::string& pattern, const std::string& tag);
        Rule regexToken(const std::string& name, const std::string& pattern, const std::string& tag);

        Rule matchChoice(const std::string& name, const RuleList& rules, const std::string& tag);
        Rule matchSequence(const std::string& name, const RuleList& rules, const std::string& tag);

        Rule zeroOrMore(const std::string& name, const RuleRef& r, const std::string& tag);
        Rule oneOrMore(const std::string& name, const RuleRef& r, const std::string& tag);
        Rule optional(const std::string& name, const RuleRef& r, const std::string& tag);
        Rule repeated(const std::string& name, const RuleRef& r, int min, int max, const std::string& tag);

        Rule delimitedSequence(const std::string& name,
                               const RuleRef&     r1,
                               const RuleRef&     delim,
                               const std::string& tag,
                               int                min = 0,
                               int                max = -1);

        // Match a sequence represented by 'pattern', but transform the output.  One use of this is
        // to verify that an ident is not a reserved word, or is a specific word.  This class takes
        // ownership of the provided result function.

        Rule transform(const std::string& name,
                       const RuleRef&     rule,
                       ResultFunction*    xform,
                       const std::string& tag);

        const Rule* findRule(const std::string& name) const;

        void setDebugMode(bool debug);

        virtual void scanGC(std::vector<HeapObject*>& queue);

    private:
        // The heap is used to manage all the objects stored here.
        memory::OctoHeap      heap_;
        GrammarParserFactory* factory_;

        // Convert this entire design to use a GC as a kind of unit test.
        std::map<std::string, Rule*> ruleSet_;
        bool                         debug_;
    };
}

#endif

