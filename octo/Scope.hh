
#ifndef __SCOPE_HH__
#define __SCOPE_HH__

#include "Atom.hh"

#include <unordered_map>
#include <memory>

class Scope {
public:
    Scope();
    ~Scope();

    PValue lookup(const Atom& a);

    PValue find(const Atom& a);

    void assign(const Atom& a, PValue v);

private:
    std::unordered_map<Atom, std::shared_ptr<Value>> scope_;
};

typedef std::shared_ptr<Scope> PScope;

#endif

