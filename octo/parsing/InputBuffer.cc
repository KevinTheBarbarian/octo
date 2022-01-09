
#include "InputBuffer.hh"

namespace parsing {
    InputBuffer::InputBuffer(CharSlice input)
        : input_(input),
          rest_ (input),
          pos_  (0)
    {
    }

    // Match a string at the current location.
    bool InputBuffer::matchString(CharSlice token)
    {
        return
            rest_.size() >= token.size() &&
            rest_.slice(0, token.size()) == token;
    }
}

