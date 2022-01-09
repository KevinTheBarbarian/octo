
#ifndef __OCTOPARSER_HH__
#define __OCTOPARSER_HH__

#include "GrammarParser.hh"
#include "ParseResult.hh"
#include "Rule.hh"

#include <unordered_set>

class OctoParser : public parsing::GrammarParser {
public:
    OctoParser();

    parsing::ParseResult parse(const general::CharSlice& data);

private:
    typedef parsing::RuleRef RuleRef;

    void matchKeyword_(const std::string& keyword, const std::string& tag);

    void delimitOrDescend(const std::string&  name,
                          const RuleRef&      rule1,
                          const RuleRef&      delim,
                          const std::string&  tag);

    void buildRules_();

    std::unordered_set<std::string> keywords_;
};

#endif

