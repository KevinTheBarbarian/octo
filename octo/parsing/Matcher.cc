
#include "Matcher.hh"

using namespace std;

namespace parsing {
    Matcher::Matcher(const std::string& tag)
        : tag_(tag)
    {
    }

    Matcher::~Matcher()
    {
    }

    const std::string& Matcher::getTag() const
    {
        return tag_;
    }
}

