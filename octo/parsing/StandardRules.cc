
/*
#include "StandardRules.hh"

#include "MatchChoice.hh"
#include "MatchRegex.hh"
#include "MatchRepeated.hh"
#include "MatchSequence.hh"
#include "MatchString.hh"
#include "Token.hh"

namespace parsing {
    Rule matchString(const string& str)
    {
        return Rule(new MatchString(str));
    }

    Rule matchRegex(const string& pattern)
    {
        return Rule(new MatchRegex(pattern));
    }

    Rule matchToken(const Rule& r)
    {
        return Rule(new Token(r));
    }

    Rule stringToken(const string& s)
    {
        return matchToken(matchString(s));
    }

    Rule regexToken(const string& rx)
    {
        return matchToken(matchRegex(rx));
    }

    Rule matchSequence(const std::initializer_list<Rule>& rules)
    {
        vector<Rule> vec(rules.begin(), rules.end());
        return new MatchSequence(vec);
    }

    Rule matchChoice(const std::initializer_list<Rule>& rules)
    {
        vector<Rule> vec(rules.begin(), rules.end());
        return new MatchChoice(vec);
    }

    Rule repeated(const Rule& r, int min, int max)
    {
        return new MatchRepeated(r, min, max);
    }

    Rule zeroOrMore(const Rule& r)
    {
        return repeated(r, 0, -1);
    }

    Rule delimitedSequence(const Rule& r1, const Rule& delim)
    {
        return matchSequence({r1, zeroOrMore(matchSequence({delim, r1}))});
    }
}

*/
