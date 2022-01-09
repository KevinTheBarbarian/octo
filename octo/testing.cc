
#include "testing.hh"

#include <iostream>

#include "DirEntry.hh"
#include "HeapRoot.hh"
#include "OctoParser.hh"
#include "Runtime.hh"
#include "StandardLibrary.hh"
#include "string_utils.hh"
#include "Time.hh"
#include "ParseTreeToCode.hh"
#include "UnitTest.hh"

#include <sstream>

using namespace std;
using namespace sys;
using namespace general;
using namespace memory;
using namespace parsing;
using namespace types;

void test1_math()
{
    Runtime r;
    addStandardFunctions(r);

    /*
    auto args = r.sequence({r.integer(51), r.integer(93)});
    auto expr = r.sequence({r.atom("+"), args});

    PValue vexpr(expr);
    auto rv = expr->eval(r, vexpr);

    int64_t result = asType<Integer>(rv)->getInteger();
    */

    auto add1 = r.sequence({r.atom("+"), r.sequence({r.integer(41), r.integer(33)})});
    auto add2 = r.sequence({r.atom("+"), r.sequence({r.integer(11), r.integer(13)})});
    auto mult = r.sequence({r.atom("*"), r.sequence({add1, add2})});

    PValue vexpr(mult);
    PValue rv = vexpr->eval(r, vexpr);

    int64_t result = asType<Integer>(rv)->getInteger();

    cout << "test1: result is " << result << endl;
}

void test2_parse()
{
    string expr = "  foo()\n  {\n  }\n";

    OctoParser parser;
    auto result = parser.parse(CharSlice(expr));

    cout << "success? " << result.isSuccess() << endl;
}

ParseResult testParse(const DirEntry& testFile)
{
    string program = testFile.readFile();

    OctoParser parser;
    auto code = parser.parse(CharSlice(program));

    cout << "Parsing (" << testFile.getPath() << "): ";

    cout << "\nContents:\n----------\n" << program << "\n----------\n";

    if (! code.isSuccess()) {
        CharSlice context = code.getContext().getRest();

        if (context.size() > 10) {
            context = context.slice(0, 10);
        }

        string printable = filterPrintable(context);

        cout << ", parsed(syntax error) [context: "
             << printable << "...]";
    } else {
        cout << "success\n" << endl;
        cout << "\nParse Tree:\n>>\n" << code.getParseTree() << "\n<<\n";
    }

    return code;
}

void testAstToCode(const ParseTree& tree, const string& astFileName)
{
    OctoConfig config;
    config.setDebugCompile(true);

    OctoHeap        heap;
    ObjectFactory   factory(heap);
    ParseTreeToCode treeToCode(config, factory);

    HeapRoot<OctoSequence> obj(heap, 0);
    obj.set(treeToCode.translate(tree));

    cout << "AstToCode: ";

    if (! obj.get()) {
        cout << "error" << endl;
        return;
    }

    cout << "success" << endl;
    cout << "\n" << endl;

    cout << "Code output:\n>>\n"
         << obj.get()
         << "\n<<\n" << endl;
}

void testProgram(const ParseTree& tree, const string& expectedFileName)
{
    DirEntry expectedFile(expectedFileName);

    if (! expectedFile.isFile()) {
        cout << ", results(n/a)";
        return;
    }

    string expectedOutput = expectedFile.readFile();
    string programOutput;

    // run the program, generating output
    // compare to expected output

    if (programOutput == expectedOutput) {
        cout << ", output(correct)";
    } else {
        cout << ", output(MISMATCH)";
    }
}

void runFileTest(const DirEntry& testFile, const string& baseName)
{
    cout << "test (" << baseName << ")" << flush;
    string expectedFile = baseName + ".expected";
    string astFile      = baseName + ".ast";

    auto code = testParse(testFile);

    if (code.isSuccess()) {
        auto ast = code.getParseTree();
        testAstToCode(ast, astFile);
        testProgram(ast, expectedFile);
    }
}

void runFileTests()
{
    cout << "\n\n--- Running file based tests ---\n\n";

    vector<DirEntry> testFiles = DirEntry("tests").readDir();

    bool found = false;

    for(DirEntry& f : testFiles) {
        if (! f.isFile()) {
            continue;
        }

        string baseName = f.getPath();

        if (chompSuffix(baseName, CharSlice(".test"))) {
            found = true;

            Time start = Time::now();
            runFileTest(f, baseName);
            Time end = Time::now();

            cout << " (time: " << (end-start).toDouble() << ")" << endl;
        }
    }

    if (! found) {
        cout << "No test files found" << endl;
    }

    cout << "\n";
}

void runUnitTests()
{
    UnitTestManager::getInstance().runAll();
}

