
#include "Runtime.hh"

PSequence Runtime::evaluateAll(PSequence& argExprs)
{
    PSequence argValues = sequence();
    argValues->reserve(argExprs->size());

    for(int i=0,e=argExprs->size(); i != e; i++) {
        PValue expr  = (*argExprs)[i];
        PValue value = expr->eval(*this, expr);
        argValues->add(value);
    }

    return argValues;
}
