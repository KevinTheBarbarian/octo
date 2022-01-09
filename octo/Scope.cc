
#include "Scope.hh"

#include "Atom.hh"
#include "verify.hh"

#include <unordered_map>
#include <memory>

using namespace general;

Scope::Scope()
{
}

Scope::~Scope()
{
}

PValue Scope::lookup(const Atom& a)
{
    PValue rv = find(a);
    verify(rv.get());
    return rv;
}

PValue Scope::find(const Atom& a)
{
    auto iter = scope_.find(a);
    return (iter != scope_.end()) ? iter->second : PValue();
}

void Scope::assign(const Atom& a, PValue v)
{
    scope_[a] = v;
}

