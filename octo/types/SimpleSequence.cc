
#include "SimpleSequence.hh"

#include "verify.hh"

using namespace std;
using namespace general;

namespace types {
    int64_t SimpleSequence::size() const
    {
        return data_.size();
    }

    const OctoObject* SimpleSequence::get(int64_t i) const
    {
        verify(i >= 0,                    "Index is negative");
        verify(i < (int64_t)data_.size(), "Index out of range");
        return data_[i];
    }

    void SimpleSequence::append(OctoObject* obj)
    {
        data_.push_back(obj);
    }

    void SimpleSequence::scanGC(vector<HeapObject*>& queue)
    {
        for(auto i=data_.begin(), e=data_.end(); i != e; i++) {
            OctoObject* obj = *i;

            if (obj) {
                queue.push_back(obj);
            }
        }
    }

    void SimpleSequence::printObject(ostream& output) const
    {
        bool first = true;
        output << "[";

        for(auto i=data_.begin(), e=data_.end(); i != e; i++) {
            if (! first) {
                output << ", ";
            }
            first = false;
            output << *i;
        }
        output << "]";
    }
}

