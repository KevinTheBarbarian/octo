
#ifndef __VERIFY_HH__
#define __VERIFY_HH__

#include <stdexcept>

namespace general {
    void verifyFailed(bool cond, const char* msg)
    {
        if (! cond) {
            throw std::runtime_error(msg);
        }
    }
}

#endif

