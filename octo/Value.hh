// This is a generic value object that represents a choice between each concrete type. Evaluation
// (execution) of the expression represented here is implemented as a lisp-like idiom, although the
// language source code isn't visually structured that way.

#ifndef __VALUE_HH__
#define __VALUE_HH__

#include "common.hh"
#include "verify.hh"

#include <memory>

class Runtime;
class Sequence;

class Value {
public:
    enum BaseType {
        ATOM = 1,
        INTEGER,
        INTRINSIC,
        SEQUENCE
    };

    Value(BaseType bt);
    virtual ~Value();

    BaseType getBaseType() const
    {
        return baseType_;
    }

    bool isType(BaseType bt) const
    {
        return baseType_ == bt;
    }

    // Apply these arguments to this object (like a function call).
    virtual std::shared_ptr<Value> eval(Runtime& runtime, std::shared_ptr<Value>& self) = 0;

    // Apply these arguments to this object (like a function call).
    virtual std::shared_ptr<Value> apply(Runtime&                   runtime,
                                         std::shared_ptr<Sequence>& argExprs) = 0;

private:
    general::NotCopyable nocopy_;
    BaseType baseType_;
};

typedef std::shared_ptr<Value> PValue;

template<class T>
std::shared_ptr<T> asType(const PValue& v)
{
    auto rv = std::dynamic_pointer_cast<T>(v);
    general::verify(rv.get());
    return rv;
}

#endif

