
#include "OctoHeap.hh"

#include <iostream>

using namespace std;

namespace memory {
    bool GC_verbose_stats = false;
    bool GC_trace         = false;

    OctoHeap::OctoHeap()
        : currentMark_(0)
    {
    }

    OctoHeap::~OctoHeap()
    {
        garbageCollect();
    }

    void OctoHeap::addRoot(HeapObject* obj)
    {
        if (GC_trace) cerr << "GC:  adding root " << obj << endl;
        roots_.insert(obj);
    }

    void OctoHeap::removeRoot(HeapObject* obj)
    {
        if (GC_trace) cerr << "GC:  removing root " << obj << endl;
        roots_.erase(obj);
    }

    void OctoHeap::manageObject(HeapObject* obj)
    {
        if (GC_trace) cerr << "GC:  managing obj " << obj << endl;
        allocated_.insert(obj);
    }

    void OctoHeap::garbageCollect()
    {
        if (GC_verbose_stats) cerr << "GC start: object count = " << allocated_.size() << endl;
        currentMark_ = (currentMark_ + 1) & 127;

        markRoots_();
        mark_();
        queue_.clear();

        sweep_();
        if (GC_verbose_stats) cerr << "GC done:  object count = " << allocated_.size() << endl;
    }

    void OctoHeap::markRoots_()
    {
        for(auto& rt : roots_) {
            queue_.push_back(&* rt);
        }
    }

    void OctoHeap::mark_()
    {
        int8_t mark = currentMark_;

        while(! queue_.empty()) {
            HeapObject* obj = queue_.back();
            queue_.pop_back();

            if (obj) {
                if (GC_trace) cerr << "GC:  marking obj " << obj << endl;
                obj->setGCMark(mark);
                obj->scanGC(queue_);
            }
        }
    }

    void OctoHeap::sweep_()
    {
        int8_t mark = currentMark_;

        auto iter = allocated_.begin();
        auto iend = allocated_.end();

        while(iter != iend) {
            HeapObject* obj = *iter;

            if (obj->getGCMark() == mark) {
                ++iter;
            } else {
                iter = allocated_.erase(iter);
                if (GC_trace) cerr << "deleting object " << obj << endl;
                delete obj;
            }
        }
    }
}

