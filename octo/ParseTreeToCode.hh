
#ifndef __PARSE_TREE_TO_CODE_HH__
#define __PARSE_TREE_TO_CODE_HH__

#include "HeapRoot.hh"
#include "ObjectFactory.hh"
#include "OctoConfig.hh"
#include "ParseTreeWalker.hh"

class ParseTreeToCode {
public:
    ParseTreeToCode(const OctoConfig& config, types::ObjectFactory& factory);

    types::OctoSequence* translate(const parsing::ParseTree& codeTree);

private:
    void parseGlobalDefnList_(const parsing::ParseTree& tree, types::OctoSequence* seq);

    void functionDefnToSeq_(const parsing::ParseTree& tree, types::OctoSequence* seq);

    void defineFunction_(const parsing::ParseTree& tree, types::OctoSequence* seq);
    void appendSignature_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    void appendParamBlock_(types::OctoSequence*      funcSeq,
                           const std::string&        tag,
                           parsing::ParseTreeWalker& walker,
                           bool                      deduced);

    types::HeapRoot<types::OctoSequence> appendParamList_(const parsing::ParseTree& tree);

    void appendParam_(const parsing::ParseTree& tree, types::OctoSequence* seq);

    void appendCodeBlock_(types::OctoSequence* funcSeq, const parsing::ParseTree& tree);
    void appendExpr_(types::OctoSequence* seq, const parsing::ParseTree& tree);
    void appendExprSeq_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    void appendLeftAssociativeExpr_(types::OctoSequence* seq, const parsing::ParseTree& tree);
    void appendRightAssociativeExpr_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    template<class LHS, class RHS>
    types::HeapRoot<types::OctoSequence> buildBinaryExpr_(const LHS&                lhs,
                                                          const parsing::ParseTree& token,
                                                          const RHS&                rhs);

    const std::string& getOpToken_(const parsing::ParseTree& tree);
    void buildOrAppend_(types::OctoSequence* dest, const parsing::ParseTree&  expr);
    void buildOrAppend_(types::OctoSequence* dest, types::OctoSequence* expr);

    void appendAccessExpr_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    template<class LHS>
    types::HeapRoot<types::OctoSequence> buildAccessExpr_(const LHS&                lhs,
                                                          const parsing::ParseTree& access);

    void appendCompareExpr_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    void appendReturnStmt_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    void appendWhileStmt_(types::OctoSequence* seq, const parsing::ParseTree& tree);

    void                 appendString_(types::OctoSequence* seq, const std::string& str);
    types::OctoSequence* appendSequence_(types::OctoSequence* seq);

    OctoConfig            config_;
    types::ObjectFactory& factory_;
};

#endif

