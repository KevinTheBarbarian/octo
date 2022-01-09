
#include "Atom.hh"

#include <unordered_map>

using namespace std;

namespace {
    unordered_map<string, size_t> atoms_;
    size_t next_atom_id_ = 1;
}

void Atom::init_(const char* value)
{
    auto iter = atoms_.find(value);

    if (iter == atoms_.end()) {
        auto map_value = make_pair(string(value), next_atom_id_++);
        iter = atoms_.insert(map_value).first;
    }

    str_ = & iter->first;
    id_  = iter->second;
}
