
#ifndef __COLLECTION_UTILS_HH__
#define __COLLECTION_UTILS_HH__

#include "verify.hh"

#include <iostream>
#include <map>
#include <set>

namespace general {
    template<class T, class M1, class M2, class M3>
    void printCollection(std::ostream& output,
                         const T&      obj,
                         const M1&     before,
                         const M2&     between,
                         const M3&     after)
    {
        bool first = true;
        output << before;

        for(auto i = obj.begin(), e = obj.end(); i != e; i++) {
            if (first) {
                first = false;
            } else {
                output << between;
            }
            output << *i;
        }

        output << after;
    }

    template<class T, class M1, class M2, class M3>
    void printCollectionOutline(std::ostream& output,
                                const T&      obj,
                                const M1&     before,
                                const M2&     between,
                                const M3&     after,
                                int           indent)
    {
        bool first = true;
        output << before;

        for(auto i = obj.begin(), e = obj.end(); i != e; i++) {
            if (first) {
                first = false;
            } else {
                output << between;
            }
            (*i).printOutline(output, indent);
        }

        output << after;
    }

    template<class K, class V>
    bool containsKey(const std::map<K, V>& c, const K& k)
    {
        return c.find(k) != c.end();
    }

    template<class K>
    bool containsKey(const std::set<K>& c, const K& k)
    {
        return c.find(k) != c.end();
    }

    template<class K>
    void insertUnique(std::set<K>& c, const K& k)
    {
        verify(! containsKey(c, k));
        c.insert(k);
    }

    template<class K>
    void removeExisting(std::set<K>& c, const K& k)
    {
        verify(containsKey(c, k));
        c.erase(k);
    }
}

#endif

