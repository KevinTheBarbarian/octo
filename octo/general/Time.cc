
#include "Time.hh"

namespace general {
    Time::Time()
        : time_{0, 0}
    {
    }

    Time Time::now()
    {
        Time t;
        clock_gettime(CLOCK_REALTIME, &t.time_);
        return t;
    }

    int64_t Time::toNano_() const
    {
        return time_.tv_sec * 1000000000 + time_.tv_nsec;
    }

    TimeDiff Time::operator-(const Time& other) const
    {
        int64_t diff = toNano_() - other.toNano_();
        return TimeDiff(diff);
    }
}

