
#include "string_utils.hh"

using namespace std;

namespace general {
    bool hasPrefix(CharSlice one, CharSlice prefix)
    {
        return startMatches(one, prefix);
    }

    bool hasSuffix(CharSlice one, CharSlice suffix)
    {
        return endMatches(one, suffix);
    }

    bool chompSuffix(string& target, CharSlice match)
    {
        bool found = hasSuffix(CharSlice(target), match);

        if (found) {
            target.resize(target.size()-match.size());
        }

        return found;
    }

    bool chompPrefix(string& target, CharSlice match)
    {
        bool found = hasPrefix(CharSlice(target), match);

        if (found) {
            target.erase(0, match.size());
        }

        return found;
    }

    string filterPrintable(const CharSlice& text)
    {
        string output;
        output.reserve(text.size()*5/4);

        for(char ch : text) {
            if (isprint(ch)) {
                output += ch;
            } else {
                output += "?";
            }
        }

        return output;
    }
}

