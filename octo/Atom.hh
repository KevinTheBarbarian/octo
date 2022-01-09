
#ifndef __ATOM_HH__
#define __ATOM_HH__

#include "Primitive.hh"

class Atom : public Primitive {
public:
    Atom(const char* value)
        : Primitive(ATOM)
    {
        init_(value);
    }

    Atom(const Atom& other)
        : Primitive(ATOM),
          str_ (other.str_),
          id_  (other.id_)
    {
    }

    size_t getId() const
    {
        return id_;
    }

    const std::string& getText() const
    {
        return *str_;
    }

private:
    void init_(const char* value);

    const std::string* str_;
    size_t             id_;
};

namespace std {
    template<>
    class hash<Atom> {
    public:
        size_t operator()(const Atom& atom) const
        {
            return atom.getId();
        }
    };
}

inline bool operator==(const Atom& a, const Atom& b)
{
    return a.getId() == b.getId();
}

typedef std::shared_ptr<Atom> PAtom;

#endif

