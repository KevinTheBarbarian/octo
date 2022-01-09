
#ifndef __OCTO_SOURCE_LIBRARY_HH__
#define __OCTO_SOURCE_LIBRARY_HH__

#include "OctoConfig.hh"
#include "OctoHeap.hh"
#include "OctoSequence.hh"

class OctoSourceLibrary : public memory::HeapObject, private general::NotCopyable {
public:
    OctoSourceLibrary(const OctoConfig& config,
                      memory::OctoHeap& heap);

    ~OctoSourceLibrary();

    void storeFunctionDefinition(const std::string&         moduleName,
                                 const std::string&         functionName,
                                 const types::OctoSequence* definition);

    virtual void scanGC(std::vector<HeapObject*>& queue);

private:
    // Config

    OctoConfig        config_;
    memory::OctoHeap& heap_;
};

#endif

