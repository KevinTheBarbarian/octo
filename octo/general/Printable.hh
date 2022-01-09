
#ifndef __PRINTABLE_HH__
#define __PRINTABLE_HH__

#include <iostream>

namespace general {
    class Printable {
    public:
        Printable();
        virtual ~Printable();

        virtual void print(std::ostream& outp) const = 0;

        virtual void debug() const;
    };

    inline std::ostream& operator<<(std::ostream& out, const Printable& v)
    {
        v.print(out);
        return out;
    }
}

#endif

