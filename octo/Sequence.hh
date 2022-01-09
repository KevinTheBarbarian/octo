
#ifndef __SEQUENCE_HH__
#define __SEQUENCE_HH__

#include "Value.hh"

#include <vector>
#include <memory>

class Scope;

class Sequence : public Value {
public:
    Sequence()
        : Value(SEQUENCE)
    {
    }

    Sequence(const std::initializer_list<PValue>& seq)
        : Value(SEQUENCE),
          contents_(seq)
    {
    }

    void reserve(size_t sz)
    {
        contents_.reserve(sz);
    }

    void add(PValue item)
    {
        contents_.push_back(item);
    }

    size_t size()
    {
        return contents_.size();
    }

    PValue operator[](size_t i)
    {
        return contents_[i];
    }

    // Evaluating a sequence means evaluating it as an expression: for example, [+, 1, 2] is a
    // sequence that is evaluated by finding whatever is bound to '+', and 'apply'ing [1, 2] to it.
    virtual PValue eval(Runtime& runtime, PValue& self);

    // Applying a sequence means running it as a function.  For example, ['square [1]] is a
    // function call.
    virtual PValue apply(Runtime&                   runtime,
                         std::shared_ptr<Sequence>& sequence);

private:
    std::shared_ptr<Scope> evaluateArgList_(Runtime&                   runtime,
                                            std::shared_ptr<Sequence>& argExprs,
                                            std::shared_ptr<Sequence>& parameters);

    void evaluateParam_(Runtime& runtime,
                        PValue   argExprs,
                        PValue   parameter,
                        Scope&   scope);

    std::vector<PValue> contents_;
};

typedef std::shared_ptr<Sequence> PSequence;

#endif

