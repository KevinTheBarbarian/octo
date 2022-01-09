
#ifndef __TIME_HH__
#define __TIME_HH__

#include <time.h>
#include <stddef.h>
#include "common.hh"

#include <algorithm>
#include <iostream>

#include <assert.h>
#include <stddef.h>
#include <string.h>

namespace general {
    class TimeDiff {
    public:
        TimeDiff(int64_t diff)
            : nano_(diff)
        {
        }

        double toDouble() const
        {
            return double(nano_/1000000000.0);
        }

    private:
        int64_t nano_;
    };

    class Time {
    public:
        Time();

        static Time now();

        TimeDiff operator-(const Time& other) const;

    private:
        int64_t toNano_() const;

        timespec time_;
    };
}

#endif

