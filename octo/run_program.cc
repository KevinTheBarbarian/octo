
#include "run_program.hh"

#include "OctoProcess.hh"

#include <iostream>

using namespace std;
using namespace general;

void runProgram(const CommandLine& cmdLine)
{
    const vector<string>& args = cmdLine.getArgs();

    if (args.size() < 1) {
        cerr << "Error: expected 1 module name for run mode, but got " << args.size() << "." << endl;
        return;
    }

    OctoConfig config;

    if (cmdLine.hasFlag("debug-compile")) {
        config.setDebugCompile(true);
    }

    if (cmdLine.hasFlag("debug-parse")) {
        config.setDebugParse(true);
    }

    config.setStartModule(args[0]);

    OctoProcess process(config);
    process.loadModules();
    process.run(args);
}

