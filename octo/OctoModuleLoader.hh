
#ifndef __OCTO_MODULE_LOADER_HH__
#define __OCTO_MODULE_LOADER_HH__

#include "HeapRoot.hh"
#include "OctoConfig.hh"
#include "OctoSequence.hh"
#include "OctoSourceLibrary.hh"
#include "ParseResult.hh"
#include "common.hh"

#include <list>
#include <set>

class OctoModuleLoader : public memory::HeapObject, private general::NotCopyable {
public:
    OctoModuleLoader(const OctoConfig&  config,
                     memory::OctoHeap&  heap,
                     OctoSourceLibrary* sourceLibrary);

    void addIncludePath(const std::string& path);

    void loadModule(const std::string& startModule, bool local);

    void loadDependencies();

    virtual void scanGC(std::vector<HeapObject*>& queue);

private:
    void importModule_(const std::string& module, bool local);

    std::string moduleNameToFileName_(const std::string& moduleName, bool local);

    std::string getModuleText_(const std::string& moduleName, const std::string& fileName);

    parsing::ParseTree textToParseTree_(const std::string& moduleName, const std::string& codeText);

    void reportErrorDetails_(const parsing::ParseResult& code);

    types::HeapRoot<types::OctoSequence> astToCodeSequence_(const parsing::ParseTree& parsed);

    void storeModuleCode_(const std::string&         moduleName,
                          const types::OctoSequence& codeSeq,
                          std::vector<std::string>&  imports);

    void processDefinition_(const std::string&         moduleName,
                            const types::OctoSequence* codeSeq,
                            std::vector<std::string>&  imports);

    void processFunctionDefinition_(const std::string&         moduleName,
                                    const types::OctoSequence* codeSeq);

    void processModuleImport_(const types::OctoSequence& codeSeq,
                              std::vector<std::string>&  imports);

    void queueImports_(const std::vector<std::string>& modules);

    // Config

    OctoConfig               config_;
    memory::OctoHeap&        heap_;
    OctoSourceLibrary*       sourceLibrary_;
    std::vector<std::string> includePaths_;

    // State

    std::list<std::string> importQueue_;
    std::set<std::string>  alreadyLoaded_;
};

#endif

