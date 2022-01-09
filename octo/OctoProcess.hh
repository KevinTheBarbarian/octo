
#ifndef __OCTO_PROCESS_HH__
#define __OCTO_PROCESS_HH__

#include "common.hh"

#include "OctoConfig.hh"
#include "OctoModuleLoader.hh"

class OctoProcess : public memory::HeapObject, private general::NotCopyable {
public:
    OctoProcess(const OctoConfig& config);
    ~OctoProcess();

    void loadModules();

    void run(const std::vector<std::string>& args);

    virtual void scanGC(std::vector<HeapObject*>& queue);

private:
    // Config

    OctoConfig  config_;
    std::string startModule_;

    // State

    memory::OctoHeap   heap_;
    OctoSourceLibrary* sourceLibrary_;
    OctoModuleLoader*  moduleLoader_;
};

#endif

