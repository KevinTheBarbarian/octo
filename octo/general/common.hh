
#ifndef __COMMON_HH__
#define __COMMON_HH__

#include <algorithm>

namespace general {
    class NotCopyable {
    public:
        NotCopyable() = default;
        NotCopyable(const NotCopyable&) = delete;
        NotCopyable& operator=(const NotCopyable&) = delete;
    };

    template<class A, class B>
    inline int compareTwo(const A& a, const B& b)
    {
        if (a < b) {
            return -1;
        }
        if (b < a) {
            return 1;
        }
        return 0;
    }

    // Compare to another slice, or anything that has .size() and [].
    template<class T, class U>
    bool equalCollections(const T& one, const U& two)
    {
        size_t sz = one.size();

        if (sz != two.size()) {
            return false;
        }

        for(size_t i=0, e=sz; i != e; i++) {
            if (one[i] != two[i]) {
                return false;
            }
        }

        return true;
    }

    template<class T, class U>
    int compareCollections(const T& one, const U& two)
    {
        int s1 = one.size(), s2 = two.size();
        int rv = 0;

        for(size_t i=0, e=std::min(s1, s2); i != e && rv == 0; i++) {
            rv = compareTwo(one[i], two[i]);
        }

        if (! rv) {
            rv = compareTwo(s1, s2);
        }

        return 0;
    }

#define UNIMPLEMENTED { throw std::runtime_error(std::string("ERROR: unimplemented code: ") + __FUNCTION__); }

}

#endif

