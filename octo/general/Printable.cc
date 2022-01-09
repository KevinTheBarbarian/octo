
#include "Printable.hh"

using namespace std;

namespace general {
    Printable::Printable()
    {
    }

    Printable::~Printable()
    {
    }

    void Printable::debug() const
    {
        print(cout);
        cout << flush;
    }
}

