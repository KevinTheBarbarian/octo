
#include "ParseTreeToCode.hh"
#include "ParseTreeWalker.hh"
#include "HeapRoot.hh"
#include "verify.hh"

using namespace general;
using namespace parsing;
using namespace std;
using namespace types;

// foo(a, b) {
//   c := a + b;
//   return c;
// }
//
// Becomes:
//
// ["define_function",
//   "foo",
//   [["a"], ["b"]],
//   *
//   [
//     [":=", "c", ["+", "a", "b"]],
//     ["return", "c"]
//   ]
// ]
//
// define-function
// name
// input arguments
// output types (empty if not specified...)
// code block

ParseTreeToCode::ParseTreeToCode(const OctoConfig& config, types::ObjectFactory& factory)
    : config_  (config),
      factory_ (factory)
{
}

OctoSequence* ParseTreeToCode::translate(const ParseTree& codeTree)
{
    HeapRoot<OctoSequence> topSeq(factory_.getHeap(), factory_.makeSequence());

    if (config_.getDebugParse()) {
        cout << "(Printing tree):\n";
        codeTree.printOutline(cout, 4);
        cout << "\n(End of tree)\n" << endl;
    }

    parseGlobalDefnList_(codeTree, topSeq.get());

    return topSeq.get();
}

void ParseTreeToCode::parseGlobalDefnList_(const ParseTree& tree, OctoSequence* seq)
{
    verify(tree.hasTag(), "top level element has no tag");
    verify(tree.getTag() == "globalDefnList", "unexpected top level tag: " + tree.getTag());

    ParseTreeWalker walker(tree);

    while(! walker.isComplete()) {
        if (walker.matchNode("funcDefn")) {
            // In the (near) future there should be other options here, such as 'import' statements and
            // class definitions.
            auto subSeq = appendSequence_(seq);
            functionDefnToSeq_(walker.getCurrent(), subSeq);
        } else {
            verify(false, "appendCodeBlock: no match");
        }
    }
}

void ParseTreeToCode::functionDefnToSeq_(const ParseTree& tree, OctoSequence* seq)
{
    verify(tree.hasTag(), "top level element has no tag");
    verify(tree.getTag() == "funcDefn", "unexpected top level tag: " + tree.getTag());

    defineFunction_(tree, seq);
}

void ParseTreeToCode::defineFunction_(const ParseTree& tree, OctoSequence* seq)
{
    appendString_(seq, "define_function");

    ParseTreeWalker walker(tree);

    walker.requireNode("ident");
    appendString_(seq, walker.getCurrent().getToken());

    if (walker.matchNode("signature")) {
        appendSignature_(seq, walker.getCurrent());
    } else {
        appendSignature_(seq, ParseTree());
    }

    if (walker.matchNode("codeBlock")) {
        appendCodeBlock_(seq, walker.getCurrent());
    } else {
        // Add an empty code block if there isn't one in the AST.
        appendSequence_(seq);
    }

    verify(walker.isComplete());
}

void ParseTreeToCode::appendString_(OctoSequence* seq, const std::string& str)
{
    seq->append(factory_.makeString(str.c_str()));
}

OctoSequence* ParseTreeToCode::appendSequence_(OctoSequence* parent)
{
    OctoSequence* newSeq = factory_.makeSequence();
    parent->append(newSeq);
    return newSeq;
}

void ParseTreeToCode::appendSignature_(OctoSequence* funcSeq, const ParseTree& tree)
{
    ParseTreeWalker walker(tree);
    appendParamBlock_(funcSeq, "inputs",  walker, false);
    appendParamBlock_(funcSeq, "outputs", walker, true);
    verify(walker.isComplete());
}

void ParseTreeToCode::appendParamBlock_(OctoSequence*    funcSeq,
                                        const string&    tag,
                                        ParseTreeWalker& walker,
                                        bool             deduced)
{
    if (walker.matchNode(tag)) {
        funcSeq->append(appendParamList_(walker.getCurrent()).get());
    } else if (deduced) {
        appendString_(funcSeq, "*");
    } else {
        appendSequence_(funcSeq);
    }
}

HeapRoot<OctoSequence> ParseTreeToCode::appendParamList_(const ParseTree& tree)
{
    HeapRoot<OctoSequence> rv(factory_.getHeap(), factory_.makeSequence());

    auto children = tree.getChildren();

    for(auto& child : children) {
        verify(child.hasTag("param"));
        appendParam_(child, rv.get());
    }

    return rv;
}

void ParseTreeToCode::appendParam_(const ParseTree& tree, OctoSequence* paramSeq)
{
    ParseTreeWalker walker(tree);
    walker.requireNode("ident");

    auto newSeq = appendSequence_(paramSeq);
    appendString_(newSeq, walker.getCurrent().getToken());

    verify(walker.isComplete());
}

// Actual code generation

void ParseTreeToCode::appendCodeBlock_(OctoSequence* seq, const ParseTree& tree)
{
    auto block = appendSequence_(seq);

    ParseTreeWalker walker(tree);

    while(! walker.isComplete()) {
        if (walker.matchNode("exprStmt")) {
            appendExpr_(block, walker.getCurrent());
        } else if (walker.matchNode("returnStmt")) {
            appendReturnStmt_(block, walker.getCurrent());
        } else if (walker.matchNode("whileStmt")) {
            appendWhileStmt_(block, walker.getCurrent());
        } else {
            verify(false, "appendCodeBlock: no match");
        }
    }

    verify(walker.isComplete());
}

void ParseTreeToCode::appendReturnStmt_(OctoSequence* seq, const ParseTree& tree)
{
    auto stmt = appendSequence_(seq);

    appendString_(stmt, "return");

    if (tree.hasChildren()) {
        verify(tree.getChildren().size() == 1, "ice: multiple returns");
        appendExpr_(stmt, tree.getChildren()[0]);
    }
}

void ParseTreeToCode::appendWhileStmt_(OctoSequence* seq, const ParseTree& tree)
{
    auto stmt = appendSequence_(seq);

    appendString_(stmt, "while");

    ParseTreeWalker walker(tree);

    walker.requireNode("compareExpr");
    appendExpr_(stmt, walker.getCurrent());

    walker.requireNode("codeBlock");
    appendCodeBlock_(stmt, walker.getCurrent());

    verify(walker.isComplete());
}

void ParseTreeToCode::appendExpr_(OctoSequence* seq, const ParseTree& tree)
{
    if (tree.hasTag("ident")) {
        appendString_(seq, tree.getToken());
    } else if (tree.hasTag("intLiteral")) {
        appendString_(seq, tree.getToken());
    } else if (tree.hasTag("stringLiteral")) {
        appendString_(seq, tree.getToken());
    } else if (tree.hasTag("exprStmt")) {
        verify(tree.getChildren().size() == 1);
        appendExpr_(seq, tree.getChildren()[0]);
    } else if (tree.hasChildren()) {
        appendExprSeq_(seq, tree);
    } else {
        verify(false, "Unknown expr type: " + tree.getTag());
    }
}

void ParseTreeToCode::appendExprSeq_(OctoSequence* seq, const ParseTree& tree)
{
    const string& tag = tree.getTag();

    if (tag == "assignExpr") {
        appendRightAssociativeExpr_(seq, tree);
    } else if (tag == "sumExpr" ||
               tag == "multExpr") {
        appendLeftAssociativeExpr_(seq, tree);
    } else if (tag == "accessExpr") {
        appendAccessExpr_(seq, tree);
    } else if (tag == "compareExpr") {
        appendCompareExpr_(seq, tree);
    } else {
        appendString_(seq, "!! not recognized: " + tag);
        return;
    }
}

void ParseTreeToCode::appendLeftAssociativeExpr_(OctoSequence* seq, const ParseTree& tree)
{
    // Parse the assignment components

    ParseTreeWalker walker(tree);

    int endPos   = walker.size();
    int startPos = 0;

    verify(endPos-startPos >= 3,  "ice: short assign");
    verify((endPos-startPos) % 2, "ice: non-odd length");

    HeapRoot<OctoSequence> exprSeq =
        buildBinaryExpr_(walker.get(startPos),
                         walker.get(startPos+1),
                         walker.get(startPos+2));

    startPos += 3;

    while(endPos > startPos) {
        verify((endPos - startPos) >= 2);

        exprSeq = buildBinaryExpr_(exprSeq.get(),
                                   walker.get(startPos),
                                   walker.get(startPos+1));
        startPos += 2;
    }

    seq->append(exprSeq.get());
}

void ParseTreeToCode::appendRightAssociativeExpr_(OctoSequence* seq, const ParseTree& tree)
{
    // Parse the assignment components

    ParseTreeWalker walker(tree);

    int endPos   = walker.size();
    int startPos = 0;

    verify(endPos-startPos >= 3,  "ice: short assign");
    verify((endPos-startPos) % 2, "ice: non-odd length");

    HeapRoot<OctoSequence> exprSeq =
        buildBinaryExpr_(walker.get(endPos-3),
                         walker.get(endPos-2),
                         walker.get(endPos-1));

    endPos -= 3;

    while(endPos > startPos) {
        verify((endPos - startPos) >= 2);

        exprSeq = buildBinaryExpr_(walker.get(endPos-2),
                                   walker.get(endPos-1),
                                   exprSeq.get());

        endPos -= 2;
    }

    seq->append(exprSeq.get());
}

template<class LHS, class RHS>
HeapRoot<OctoSequence> ParseTreeToCode::buildBinaryExpr_(const LHS&       lhs,
                                                         const ParseTree& token,
                                                         const RHS&       rhs)
{
    HeapRoot<OctoSequence> expr(factory_.getHeap(), factory_.makeSequence());

    // FIXME: Should be 'appendIdent' not appendString.
    appendString_(expr.get(), getOpToken_(token));
    buildOrAppend_(expr.get(), lhs);
    buildOrAppend_(expr.get(), rhs);
    return expr;
}

void ParseTreeToCode::buildOrAppend_(OctoSequence* dest, const ParseTree& expr)
{
    appendExpr_(dest, expr);
}

void ParseTreeToCode::buildOrAppend_(OctoSequence* dest, OctoSequence* expr)
{
    dest->append(expr);
}

const string& ParseTreeToCode::getOpToken_(const ParseTree& tree)
{
    if (tree.hasTag("sumOp")    ||
        tree.hasTag("multOp")   ||
        tree.hasTag("assignOp") ||
        tree.hasTag("compareOp")) {

        auto children = tree.getChildren();
        verify(children.size() == 1, "ice: strange op");
        return children[0].getToken();
    }

    verify(false, "Unhandled tag: " + tree.getTag());

    static string noWarn;
    return noWarn;
}

void ParseTreeToCode::appendAccessExpr_(OctoSequence* seq, const ParseTree& tree)
{
    // Parse the assignment components

    ParseTreeWalker walker(tree);

    int endPos   = walker.size();
    int startPos = 0;

    verify(endPos-startPos >= 2,  "ice: short access");

    HeapRoot<OctoSequence> exprSeq =
        buildAccessExpr_(walker.get(startPos),
                         walker.get(startPos+1));

    startPos += 2;

    while(startPos < endPos) {
        exprSeq = buildAccessExpr_(exprSeq.get(),
                                   walker.get(startPos));

        startPos ++;
    }

    seq->append(exprSeq.get());
}

template<class LHS>
HeapRoot<OctoSequence> ParseTreeToCode::buildAccessExpr_(const LHS&       lhs,
                                                         const ParseTree& access)
{
    HeapRoot<OctoSequence> expr(factory_.getHeap(), factory_.makeSequence());

    string accessType = access.getTag();

    if (accessType == "funcCall") {
        buildOrAppend_(expr.get(), lhs);
 
        for(const ParseTree& arg : access.getChildren()) {
            appendExpr_(expr.get(), arg);
        }
    } else {
        verify(false, "Unhandled access type: " + accessType);
    }

    return expr;
}

void ParseTreeToCode::appendCompareExpr_(OctoSequence* seq, const ParseTree& tree)
{
    // Parse the assignment components

    ParseTreeWalker walker(tree);

    verify(walker.size() == 3,  "ice: unrecognized compare");

    HeapRoot<OctoSequence> exprSeq =
        buildBinaryExpr_(walker.get(0),
                         walker.get(1),
                         walker.get(2));

    seq->append(exprSeq.get());
}

