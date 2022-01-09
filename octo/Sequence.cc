
#include "Sequence.hh"

#include "Atom.hh"
#include "Runtime.hh"

using namespace std;
using namespace general;

// Evaluating a sequence means assuming it is a function call, and evaluating that.
PValue Sequence::eval(Runtime& runtime, PValue& self)
{
    verify(size() == 2);

    PAtom     atom = asType<Atom>((*this)[0]);
    PSequence args = asType<Sequence>((*this)[1]);

    auto runnable = runtime.lookup(*atom);
    verify(runnable.get());

    return runnable->apply(runtime, args);
}

// Applying a sequence means treating it as the code to a function.
PValue Sequence::apply(Runtime& runtime, PSequence& argExprs)
{
    verify(size() == 2);

    PSequence parameters = asType<Sequence>((*this)[0]);
    PSequence codeBlock  = asType<Sequence>((*this)[1]);

    PScope scope = evaluateArgList_(runtime, argExprs, parameters);

    // FIXME
    // auto runnable = runtime.lookup(*atom);
    // verify(runnable.get());

    // return runnable->apply(runtime, args);
    verify(false);
    return PValue();
}

PScope Sequence::evaluateArgList_(Runtime&   runtime,
                                  PSequence& argExprs,
                                  PSequence& parameters)
{
    verify(argExprs->size() == parameters->size(), "Incorrect number of arguments");

    PScope newScope(new Scope);

    for(int i = 0, e = argExprs->size(); i != e; i++) {
        evaluateParam_(runtime, (*argExprs)[i], (*parameters)[i], *newScope);
    }

    return newScope;
}

namespace {
    struct ArgTraits {
        ArgTraits()
            : evaluated(true)
        {
        }

        void addModifier(const Atom& modifier)
        {
            static Atom a_unevaluated("unevaluated");

            if (modifier == a_unevaluated) {
                evaluated = false;
            } else {
                verify(false, string("Unknown modifier: ") + modifier.getText());
            }
        }

        PAtom argName;
        bool  evaluated;
    };

    void computeArgTraits(PValue& parameter, ArgTraits& traits)
    {
        switch(parameter->getBaseType()) {
        case Value::ATOM:
            traits.argName = asType<Atom>(parameter);
            break;

        case Value::SEQUENCE:
            {
                PSequence seq = asType<Sequence>(parameter);
                traits.argName = asType<Atom>((*seq)[0]);

                for(int i = 1, e = seq->size(); i != e; i++) {
                    traits.addModifier(*asType<Atom>((*seq)[i]));
                }
            }
            break;

        default:
            verify(false, "Unexpected type in parameter list");
        }
    }
}

void Sequence::evaluateParam_(Runtime& runtime,
                              PValue   expr,
                              PValue   parameter,
                              Scope&   scope)
{
    ArgTraits traits;
    computeArgTraits(parameter, traits);

    PValue arg = expr;
    if (traits.evaluated) {
        arg = eval(runtime, arg);
    }

    scope.assign(*traits.argName, arg);
}

