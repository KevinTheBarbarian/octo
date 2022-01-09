
#include "StandardLibrary.hh"
#include "Add.hh"
#include "Subtract.hh"
#include "Multiply.hh"
#include "Divide.hh"
#include "Runtime.hh"

template<class T>
void defineGlobal(const char* name, Runtime& r)
{
    r.globalScope().assign(*r.atom(name), PValue(new T));
}

void addStandardFunctions(Runtime& r)
{
    defineGlobal<Add>("+", r);
    defineGlobal<Subtract>("-", r);
    defineGlobal<Multiply>("*", r);
    defineGlobal<Divide>("/", r);
}

