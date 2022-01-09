
#include "OctoProcess.hh"

using namespace std;

OctoProcess::OctoProcess(const OctoConfig& config)
    : config_       (config),
      sourceLibrary_(new OctoSourceLibrary(config, heap_)),
      moduleLoader_ (new OctoModuleLoader(config, heap_, sourceLibrary_))
{
    heap_.addRoot(this);
    heap_.manageObject(sourceLibrary_);
    heap_.manageObject(moduleLoader_);
}

OctoProcess::~OctoProcess()
{
    heap_.removeRoot(this);
}

void OctoProcess::loadModules()
{
    moduleLoader_->loadModule(config_.getStartModule(), true);
    moduleLoader_->loadDependencies();
}

void OctoProcess::run(const std::vector<std::string>& args)
{
    UNIMPLEMENTED;
}

void OctoProcess::scanGC(std::vector<HeapObject*>& queue)
{
    queue.push_back(sourceLibrary_);
    queue.push_back(moduleLoader_);
}

