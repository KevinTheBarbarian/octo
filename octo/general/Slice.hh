
#ifndef __SLICE_HH__
#define __SLICE_HH__

#include "common.hh"

#include <algorithm>
#include <iostream>

#include <assert.h>
#include <stddef.h>
#include <string.h>

namespace general {
    template<typename T>
    class Slice {
    public:
        Slice()
            : start_(0), size_(0)
        {
        }

        Slice(const T* data, size_t length)
            : start_(data), size_(length)
        {
        }

        explicit Slice(const char* data)
            : start_(data), size_(strlen(data))
        {
        }

        template<typename C>
        explicit Slice(const C& obj)
            : start_(& obj[0]), size_(obj.size())
        {
        }

        const T* data() const
        {
            return start_;
        }

        size_t size() const
        {
            return size_;
        }

        bool empty() const
        {
            return ! size_;
        }

        const T& operator[](size_t i) const
        {
            return start_[i];
        }

        Slice<T> slice(size_t i, size_t j) const
        {
            assert(i <= j);
            assert(j <= size_);

            return Slice<T>(start_ + i, j-i);
        }

        Slice<T> removeFront(size_t bytes) const
        {
            assert(bytes <= size_);
            return slice(bytes, size_);
        }

        Slice<T> removeBack(size_t bytes) const
        {
            assert(bytes <= size_);
            return slice(0, size_ - bytes);
        }

        template<typename OBJ>
        void toObject(OBJ& obj)
        {
            obj.assign(data(), size());
        }

        template<typename OBJ>
        OBJ toObject()
        {
            OBJ obj;
            toObject(obj);
            return obj;
        }

        size_t find(const T& element) const
        {
            for(int i=0, e=size_; i != e; i++) {
                if (start_[i] == element) {
                    return i;
                }
            }
            return npos;
        }

        size_t find(Slice<T> sub) const
        {
            // Smarter algorithms exist, esp. for longer sequences.
            for(int i=0, e=size()-sub.size(); i <= e; i++) {
                if (sub == slice(i, i + sub.size())) {
                    return i;
                }
            }
            return npos;
        }

        size_t find(const char* sub1) const
        {
            Slice<T> sub(sub1);
            return find(sub);
        }

        const static size_t npos = (size_t) -1;

        class SliceIterator {
        public:
            SliceIterator()
            {
            }

            SliceIterator(const Slice<T>& range)
                : range_(range)
            {
            }

            bool operator==(const SliceIterator& other) const
            {
                return range_ == other.range_;
            }

            bool operator!=(const SliceIterator& other) const
            {
                return range_ != other.range_;
            }

            const SliceIterator& operator++()
            {
                assert(! range_.empty());
                range_ = range_.slice(1, range_.size());
                return *this;
            }

            char operator*() const
            {
                return range_[0];
            }

        private:
            Slice<T> range_;
        };

        SliceIterator begin() const
        {
            return SliceIterator(*this);
        }

        SliceIterator end() const
        {
            return SliceIterator();
        }

    private:
        const T* start_;
        size_t   size_;
    };

    template<typename T, typename U>
    bool operator==(const Slice<T>& one, const Slice<U>& two)
    {
        return equalCollections(one, two);
    }

    template<typename T>
    bool operator==(const Slice<T>& one, const Slice<T>& two)
    {
        return equalCollections(one, two);
    }

    template<typename T, typename U>
    bool operator!=(const Slice<T>& one, const Slice<U>& two)
    {
        return ! (one == two);
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const Slice<T>& sl)
    {
        for(int i=0, e=sl.size(); i != e; i++) {
            out << sl[i];
        }
        return out;
    }

    template<typename T, typename U>
    bool startMatches(Slice<T> one, const U& prefixObj)
    {
        Slice<T> prefix(prefixObj);

        return
            one.size() >= prefix.size() &&
            prefix == one.slice(0, prefix.size());
    }

    template<typename T, typename U>
    bool endMatches(Slice<T> one, const U& suffixObj)
    {
        Slice<T> suffix(suffixObj);

        return
            one.size() >= suffix.size() &&
            suffix == one.slice(one.size()-suffix.size(), one.size());
    }

    typedef Slice<char> CharSlice;

    inline std::ostream& operator<<(std::ostream& out, const CharSlice& sl)
    {
        out.write(sl.data(), sl.size());
        return out;
    }

    // const char* helper operations

    inline bool operator==(const CharSlice& one, const char* two)
    {
        return one == CharSlice(two);
    }

    // std::string helper operations

    inline std::string& operator+=(std::string& one, CharSlice two)
    {
        one.append(two.data(), two.size());
        return one;
    }
}

#endif

