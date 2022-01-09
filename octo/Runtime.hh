
#ifndef __RUNTIME_HH__
#define __RUNTIME_HH__

#include "Scope.hh"
#include "ValueFactory.hh"

class Runtime : public ValueFactory {
public:
    Runtime()
        : stackIndex_(-1)
    {
        pushStack_();
    }

    Scope& globalScope()
    {
        return *stack_[0];
    }

    Scope& currentScope()
    {
        return *stack_[stackIndex_];
    }

    void compute();

    PValue lookup(const Atom& a)
    {
        PValue rv(currentScope().lookup(a));

        if ((! rv.get()) && stackIndex_ > 0) {
            rv = globalScope().lookup(a);
        }

        return rv;
    }

    // Helper function -- takes a list of expressions and produces a list of their evaluations.
    PSequence evaluateAll(PSequence& argExprs);

private:
    void pushStack_()
    {
        stack_.push_back(PScope(new Scope));
        stackIndex_++;
    }

    size_t stackIndex_ = 0;
    std::vector<PScope> stack_;
};

#endif

