
#include "BinaryOperator.hh"
#include "Runtime.hh"

using namespace std;
using namespace general;

PValue BinaryOperator::apply(Runtime& runtime, PSequence& argExprs)
{
    verify(argExprs->size() == 2, "Binary operator called for non-binary case");

    PSequence argValues = runtime.evaluateAll(argExprs);
    auto one = (*argValues)[0];
    auto two = (*argValues)[1];
    return combine(runtime, one, two);
}

