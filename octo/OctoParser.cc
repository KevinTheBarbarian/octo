
#include "OctoParser.hh"
#include "ParseResult.hh"
#include "MatchRegex.hh"
#include "MatchString.hh"
#include "StandardRules.hh"
#include "UnitTest.hh"

#include <set>

using namespace general;
using namespace parsing;

class ReservedWords {
public:
    ReservedWords()
        : words_{"return"}
    {
    }

    bool includes(const string& word) const
    {
        return words_.find(word) != words_.end();
    }

private:
    set<string> words_;
};

class NotReserved : public ResultFunction {
public:
    virtual void print(ostream& outp) const
    {
        outp << "(not a reserved word)";
    }

    virtual ParseResult operator()(ParseResult input)
    {
        ParseTree tree = input.getParseTree();

        if (tree.hasToken() && reserved_.includes(tree.getToken())) {
            return ParseResult::fail(input.getContext());
        }

        return input;
    }

    virtual void scanGC(std::vector<HeapObject*>& queue)
    {
    }

private:
    const ReservedWords reserved_;
};

class Keyword : public ResultFunction {
public:
    Keyword(const string& word)
        : word_(word)
    {
    }

    virtual void print(ostream& outp) const
    {
        outp << "(matches word '" << word_ << "')";
    }

    virtual ParseResult operator()(ParseResult input)
    {
        ParseTree tree = input.getParseTree();

        if (tree.hasToken() && tree.getToken() == word_) {
            return input;
        }

        return ParseResult::fail(input.getContext());
    }

    virtual void scanGC(std::vector<HeapObject*>& queue)
    {
    }

private:
    const string word_;
};

OctoParser::OctoParser()
{
    buildRules_();
}

void OctoParser::matchKeyword_(const string& keyword, const string& tag)
{
    transform(keyword, "hiddenIdent", new Keyword(keyword), tag);
}

void OctoParser::delimitOrDescend(const string&  name,
                                  const RuleRef& rule1,
                                  const RuleRef& delim,
                                  const string&  tag)
{
    string seqName = name + "_del";
    delimitedSequence(seqName, rule1, delim, name, 1, -1);
    matchChoice(name, {seqName, rule1}, "");
}

void OctoParser::buildRules_()
{
    // Punctuation and similar.  In this grammar, names starting with '#' are 'visible' versions of
    // symbols that normally hide.

    stringToken("#[", "[", "[]");
    stringToken("]");

    stringToken("{", "");
    stringToken("}", "");

    stringToken("#(", "(", "()");
    stringToken("(", "");
    stringToken(")", "");

    stringToken(";", "");
    stringToken(",", "");

    stringToken("->", "");

    stringToken("+");
    stringToken("-");
    stringToken("*");
    stringToken("/");
    stringToken("%");

    stringToken("<");
    stringToken("<=");
    stringToken("==");
    stringToken("!=");
    stringToken(">=");
    stringToken(">");

    stringToken("=");
    stringToken(":=");
    stringToken("+=");
    stringToken("-=");
    stringToken("*=");
    stringToken("/=");
    stringToken("%=");

    // More interesting tokens

    string identPattern = "[a-zA-Z_][a-zA-Z0-9_]*";

    regexToken("ident",       identPattern, "ident");
    regexToken("hiddenIdent", identPattern, "");
    matchRegex("whitespace", "[ \t\n]+",    "");

    transform("name", "ident", new NotReserved, "name");

    matchKeyword_("return", "");
    matchKeyword_("while", "");

    // Constructive terms

    matchChoice("compareOp", {"<", "<=", "==", "!=", ">=", ">"}, "compareOp");
    matchChoice("sumOp",     {"+", "-"}, "sumOp");
    matchChoice("multOp",    {"*", "/", "%"}, "multOp");

    matchChoice("assignOp", {":=", "=", "+=", "-=", "*=", "/=", "%="}, "assignOp");

    // Constructive rules

    oneOrMore("globalDefnList", "globalDefn", "globalDefnList");

    matchChoice("globalDefn", {"funcDefn"}, "");

    matchSequence("funcDefn", {"name", "signature", "codeBlock"}, "funcDefn");

    matchSequence("signature", {"inputs", "optionalOutputs"}, "signature");

    matchSequence("inputs", {"parenParamList"}, "inputs");

    matchSequence("parenParamList", {"(", "paramList", ")"}, "");

    // Outputs might be typed, but need not be named, and inputs are named, and only optionally
    // typed.  However, we can leave the names on the outputs as a documentation technique, as with
    // C++ function signatures.

    optional("optionalOutputs", "returnDecl", "outputs");

    matchSequence("returnDecl", {"->", "parenParamList"}, "");

    optional("paramList", "paramList1", "");

    delimitedSequence("paramList1", "param", ",", "");

    matchSequence("param", {"name"}, "param");


    matchSequence("codeBlock", {"{", "statementList", "}"}, "codeBlock");

    zeroOrMore("statementList", "statement", "");

    matchChoice("statement", {"returnStmt", "whileStmt", "exprStmt"}, "");

    matchSequence("returnStmt", {"return", "expr", ";"}, "returnStmt");

    matchSequence("whileStmt", {"while", "(", "expr", ")", "codeBlock"}, "whileStmt");

    matchSequence("exprStmt", {"expr", ";"}, "exprStmt");

    matchChoice("expr", {"compareExpr", "assignExpr"}, "");

    matchSequence("compareExpr", {"assignExpr", "compareOp", "assignExpr"}, "compareExpr");

    delimitOrDescend("assignExpr", "sumExpr", "assignOp", "assignExpr");

    delimitOrDescend("sumExpr", "multExpr", "sumOp", "sumExpr");

    delimitOrDescend("multExpr", "accessExpr", "multOp", "multExpr");

    matchChoice("accessExpr", {"accessExpr_seq", "valueExpr"}, "");

    matchSequence("accessExpr_seq", {"valueExpr", "accessOpList"}, "accessExpr");

    oneOrMore("accessOpList", "accessOp", "");

    matchChoice("accessOp", {"funcCall"}, ""); // also array index and member access (x.y)

    matchSequence("funcCall", {"(", "funcArgs", ")"}, "funcCall"); // also array index, member access (x.y)

    matchSequence("funcArgs", {"exprList"}, "");

    optional("exprList", "exprList1", "");

    delimitedSequence("exprList1", "expr", ",", "");

    matchChoice("valueExpr", {"name", "literal"}, "");

    matchChoice("literal", {"intLiteral", "stringLiteral"}, "");

    regexToken("intLiteral", "-?[0-9]+", "intLiteral");

    // Due to multi-layer escaping, we need to use '\\\\' to mean a literal backslash in the regular
    // expression and \\\" for a literal double-quote character.

    regexToken("stringLiteral", "\"(([^\\\\\\\"])|(\\\\\\\\)|(\\\\\\\")|(\\\\[abfnrtv']))*\"", "stringLiteral");
}

ParseResult OctoParser::parse(const CharSlice& text)
{
    return parseSlice("globalDefnList", "whitespace", text);
}

DEFINE_UNIT_TEST(OctoParser: Simple Function)
{
    OctoParser parser;
    CharSlice simple("\nfn(a, b) { c := a + b; d := 123; e := \"hello\\n\"; return c + d + e; }\n");

    auto pr = parser.parse(simple);

    assertTrue(pr.isSuccess());
}

