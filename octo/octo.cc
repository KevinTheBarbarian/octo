
#include "CommandLine.hh"

#include "testing.hh"
#include "run_program.hh"

#include <iostream>

using namespace std;
using namespace general;

namespace {
    string
        s_help          = "help",
        s_debug_compile = "debug-compile",
        s_debug_parse   = "debug-parse",
        s_file_tests    = "file-test",
        s_unit_tests    = "unit-test";
}

void createUI(CommandLine& cmdLine)
{
    cmdLine.addFlag(s_help,          "Print this help message.");
    cmdLine.addFlag(s_file_tests,    "Run the file test suite, then exit.");
    cmdLine.addFlag(s_unit_tests,    "Run the unit tests, then exit.");
    cmdLine.addFlag(s_debug_compile, "Output debug information during compilation.");
    cmdLine.addFlag(s_debug_parse,   "Output debug information for parsing process.");
}

void chooseMode(const CommandLine& cmdLine)
{
    if (cmdLine.hasFlag(s_help)) {
        cmdLine.printHelpMessage("octo", "<programFile>");
        return;
    }

    if (cmdLine.hasFlag(s_file_tests)) {
        runFileTests();
        return;
    }

    if (cmdLine.hasFlag(s_unit_tests)) {
        runUnitTests();
        return;
    }

    runProgram(cmdLine);
}

int main(int argc, char** argv)
{
    try {
        CommandLine cmdLine;
        createUI(cmdLine);
        cmdLine.parse(argc, argv);

        chooseMode(cmdLine);
    }
    catch(exception& err) {
        cout << "ERROR: " << err.what() << endl;
    }

    return 0;
}

