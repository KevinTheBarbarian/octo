
#include "UnitTest.hh"

#include "collection_utils.hh"
#include "verify.hh"

#include <iostream>

using namespace general;
using namespace std;

namespace general {
    // UnitTest

    UnitTest::UnitTest()
    {
        UnitTestManager::getInstance().addTest(this);
    }

    UnitTest::~UnitTest()
    {
        UnitTestManager::getInstance().removeTest(this);
    }

    bool UnitTest::doTest()
    {
        cout << "Unit Test [" << getName() << "]: " << flush;

        string message;
        message.reserve(256);

        bool rv = false;

        try {
            test();
            message = "success";
            rv = true;
        }
        catch(std::exception& e) {
            message.assign("ERROR: ");
            message.append(e.what());
            rv = false;
        }

        cout << message << endl;
        return rv;
    }

    void UnitTest::assertTrue(bool cond, const string& msg)
    {
        verify(cond, msg);
    }

    // UnitTestManager

    UnitTestManager::UnitTestManager()
    {
    }

    UnitTestManager::~UnitTestManager()
    {
    }

    UnitTestManager& UnitTestManager::getInstance()
    {
        static UnitTestManager instance;
        return instance;
    }

    void UnitTestManager::runAll()
    {
        cout << "\n--- Starting language unit tests ---\n\n";

        for(auto& ut : tests_) {
            ut->doTest();
        }

        cout << "\n--- Done language unit tests ---\n\n";
    }

    void UnitTestManager::addTest(UnitTest* test)
    {
        insertUnique(tests_, test);
    }

    void UnitTestManager::removeTest(UnitTest* test)
    {
        removeExisting(tests_, test);
    }
}

