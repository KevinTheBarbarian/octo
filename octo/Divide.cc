
#include "Divide.hh"
#include "Runtime.hh"

using namespace std;
using namespace general;

PValue Divide::combine(Runtime& runtime, const PValue& a, const PValue& b)
{
    auto one = asType<Integer>(a);
    auto two = asType<Integer>(b);

    int64_t div = two->getInteger();
    verify(div, "divide by zero");

    return runtime.integer(one->getInteger() / div);
}
