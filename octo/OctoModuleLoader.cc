
#include "OctoModuleLoader.hh"

#include "DirEntry.hh"
#include "ObjectFactory.hh"
#include "OctoParser.hh"
#include "OctoSequence.hh"
#include "ParseResult.hh"
#include "ParseTreeToCode.hh"

#include "collection_utils.hh"
#include "string_utils.hh"

using namespace std;
using namespace general;
using namespace parsing;
using namespace sys;
using namespace types;

OctoModuleLoader::OctoModuleLoader(const OctoConfig&  config,
                                   memory::OctoHeap&  heap,
                                   OctoSourceLibrary* sourceLibrary)
    : config_       (config),
      heap_         (heap),
      sourceLibrary_(sourceLibrary)
{
}

void OctoModuleLoader::loadModule(const string& moduleName, bool local)
{
    importModule_(moduleName, local);
}

void OctoModuleLoader::loadDependencies()
{
    while(! importQueue_.empty()) {
        string nextModule = importQueue_.front();
        importModule_(nextModule, false);
        importQueue_.pop_front();
    }
}

void OctoModuleLoader::importModule_(const std::string& moduleName, bool local)
{
    if (containsKey(alreadyLoaded_, moduleName)) {
        return;
    }

    alreadyLoaded_.insert(moduleName);

    string     fileName = moduleNameToFileName_(moduleName, local);
    string     codeText = getModuleText_(moduleName, fileName);
    ParseTree  codeAst  = textToParseTree_(moduleName, codeText);

    vector<string> imports;
    HeapRoot<OctoSequence> codeSeq = astToCodeSequence_(codeAst);

    storeModuleCode_(moduleName, *codeSeq, imports);
    queueImports_(imports);
}

std::string OctoModuleLoader::moduleNameToFileName_(const std::string& moduleName, bool local)
{
    const static string extn = ".oct";

    string fileName = moduleName;

    if (local && DirEntry(fileName).isFile()) {
        return fileName;
    }

    fileName += extn;

    if (DirEntry(fileName).isFile()) {
        return fileName;
    }

    for(auto p : includePaths_) {
        string combined = DirEntry::combinePaths(p, fileName);

        if (DirEntry(combined).isFile()) {
            return combined;
        }
    }

    return "";
}

string OctoModuleLoader::getModuleText_(const string& moduleName, const string& fileName)
{
    DirEntry moduleFile(fileName);

    verify(moduleFile.exists(), "Cannot find module '" + moduleName + "'");
    verify(moduleFile.isFile(), "Module object '" + fileName + "' is not a file");

    return moduleFile.readFile();
}

ParseTree OctoModuleLoader::textToParseTree_(const string& moduleName, const string& codeText)
{
    OctoParser parser;

    if (config_.getDebugParse()) {
        parser.setDebugMode(true);
    }

    auto code = parser.parse(CharSlice(codeText));

    if (! code.isSuccess()) {
        reportErrorDetails_(code);
        verify(code.isSuccess(), "ERROR: compile failed for module '" + moduleName + "'");
    }

    return code.getParseTree();
}

void OctoModuleLoader::reportErrorDetails_(const ParseResult& code)
{
    size_t maxText = 20;

    Context   context = code.getContext();
    CharSlice nearby  = context.getRest();

    if (nearby.size() > maxText) {
        nearby = nearby.slice(0, maxText);
    }

    size_t line      = context.getLine();
    size_t offset    = context.getOffset();
    string printable = filterPrintable(nearby);

    cout << "\n Syntax error at L" << line << ":" << offset << ", near '" << printable << "'...\n";
}

HeapRoot<OctoSequence> OctoModuleLoader::astToCodeSequence_(const ParseTree& tree)
{
    ObjectFactory   factory(heap_);
    ParseTreeToCode treeToCode(config_, factory);

    HeapRoot<OctoSequence> obj(heap_, 0);
    obj.set(treeToCode.translate(tree));

    return obj;
}

void OctoModuleLoader::storeModuleCode_(const string&       moduleName,
                                        const OctoSequence& codeSeq,
                                        vector<string>&     imports)
{
    if (config_.getDebugCompile()) {
        cout << "Module '" << moduleName << "' has " << codeSeq.size() << " definitions.\n";
    }

    for(int64_t i=0, e=codeSeq.size(); i != e; i++) {
        const OctoSequence* defn = dynamic_cast<const OctoSequence*>(codeSeq.get(i));
        verify(defn, "Global definition is not a sequence");

        if (config_.getDebugCompile()) {
            cout << "  Definition#" << i << ": " << *defn << "\n";
        }

        processDefinition_(moduleName, defn, imports);
    }

    if (config_.getDebugCompile()) {
        cout << endl;
    }
}

void OctoModuleLoader::processDefinition_(const string&       moduleName,
                                          const OctoSequence* defn,
                                          vector<string>&     imports)
{
    verify(defn->size() >= 1, "Global definition is too short");

    const OctoString* typeToken = dynamic_cast<const OctoString*>(defn->get(0));
    verify(typeToken, "Global definition does not start with a string");

    if (typeToken->equalTo("define_function")) {
        processFunctionDefinition_(moduleName, defn);
    } else if (typeToken->equalTo("import_module")) {
        processModuleImport_(*defn, imports);
    }
}

void OctoModuleLoader::processFunctionDefinition_(const std::string&         moduleName,
                                                  const types::OctoSequence* defn)
{
    verify(defn->size() >= 2, "Function definition is too short");

    const OctoString* functionName = dynamic_cast<const OctoString*>(defn->get(1));
    verify(functionName, "Global definition does not start with a string");

    string funcName;
    functionName->toCppString(funcName);

    sourceLibrary_->storeFunctionDefinition(moduleName, funcName, defn);
}

void OctoModuleLoader::processModuleImport_(const types::OctoSequence& codeSeq,
                                            std::vector<std::string>&  imports)
{
    UNIMPLEMENTED;
}

void OctoModuleLoader::queueImports_(const std::vector<std::string>& imports)
{
    importQueue_.insert(importQueue_.end(), imports.begin(), imports.end());
}

void OctoModuleLoader::scanGC(std::vector<HeapObject*>& queue)
{
    queue.push_back(sourceLibrary_);
}

