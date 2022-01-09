
#ifndef __VERIFY_HH__
#define __VERIFY_HH__

#include <stdexcept>

namespace general {
    extern void verifyFailed(bool cond, const char* msg);

    inline void verify(bool cond, const char* msg)
    {
        if (! cond) {
            verifyFailed(cond, msg);
        }
    }

    inline void verify(bool cond, const std::string& msg)
    {
        verify(cond, msg.c_str());
    }

    inline void verify(bool cond)
    {
        verify(cond, "verification failed");
    }
}

#endif

