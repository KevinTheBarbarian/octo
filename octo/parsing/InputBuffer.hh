
#ifndef __INPUT_BUFFER_HH__
#define __INPUT_BUFFER_HH__

#include "Slice.hh"

namespace parsing {
    class InputBuffer {
    public:
        InputBuffer(CharSlice input);

        // Match a string at the current location.
        bool matchString(CharSlice string);

    private:
        general::NotCopyable nocopy_;

        CharSlice input_;
        CharSlice rest_;
        int       pos_;
    };
}

#endif

