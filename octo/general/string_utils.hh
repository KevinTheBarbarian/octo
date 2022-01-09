
#ifndef __STRING_UTILS_HH__
#define __STRING_UTILS_HH__

#include "Slice.hh"

namespace general {
    bool hasPrefix(CharSlice one, CharSlice prefix);

    bool hasSuffix(CharSlice one, CharSlice suffix);

    bool chompPrefix(std::string& target, CharSlice match);

    bool chompSuffix(std::string& target, CharSlice match);

    std::string filterPrintable(const CharSlice& text);
}

#endif

