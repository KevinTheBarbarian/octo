
#include "OctoSourceLibrary.hh"

using namespace std;
using namespace types;

OctoSourceLibrary::OctoSourceLibrary(const OctoConfig& config,
                                     memory::OctoHeap& heap)
    : config_(config),
      heap_  (heap)
{
    heap_.addRoot(this);
}

OctoSourceLibrary::~OctoSourceLibrary()
{
    heap_.removeRoot(this);
}

void OctoSourceLibrary::storeFunctionDefinition(const string&       moduleName,
                                                const string&       functionName,
                                                const OctoSequence* definition)
{
    UNIMPLEMENTED;
}

void OctoSourceLibrary::scanGC(std::vector<HeapObject*>& queue)
{
    // FIXME: This has to scan the map of stuff that is stored here, e.g. by storeFunctionDefinition
    // and whatever else lives here.
}

