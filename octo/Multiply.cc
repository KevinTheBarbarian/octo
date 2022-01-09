
#include "Multiply.hh"
#include "Runtime.hh"

using namespace std;

PValue Multiply::combine(Runtime& runtime, const PValue& a, const PValue& b)
{
    auto one = asType<Integer>(a);
    auto two = asType<Integer>(b);
    return runtime.integer(one->getInteger() * two->getInteger());
}
