
#ifndef __UNIT_TEST_HH__
#define __UNIT_TEST_HH__

#include "common.hh"

#include <memory>
#include <set>
#include <sstream>

namespace general {
    class UnitTest : private NotCopyable {
    public:
        UnitTest();
        virtual ~UnitTest();

        void assertTrue(bool cond, const std::string& msg);

        void assertTrue(bool cond)
        {
            assertTrue(cond, "assertTrue failed");
        }

        void assertFalse(bool cond, const std::string& msg = "assertFalse failed")
        {
            assertTrue(! cond, msg);
        }

        template<class T, class U>
        void assertEqual(const T& a, const U& b)
        {
            if (a != b) {
                std::ostringstream oss;
                oss << "assertEqual failed; " << a << " != " << b;

                assertTrue(false, oss.str());
            }
        }

        bool doTest();

        virtual const char* getName() = 0;
        virtual void        test() = 0;
    };

    class UnitTestManager {
    public:
        static UnitTestManager& getInstance();

        void addTest(UnitTest* test);
        void removeTest(UnitTest* test);

        void runAll();

    private:
        UnitTestManager();
        ~UnitTestManager();

        std::set< UnitTest* > tests_;
    };

#define UT_LINE_NUMBER() __LINE__
#define UT_CONCAT_STRINGS(A, B) A ## B
#define UT_UNIQUE_NAME(BASE, LN) UT_CONCAT_STRINGS(BASE, LN)

#define DEFINE_UNIT_TEST(NAME)                                          \
    static class UT_UNIQUE_NAME(UNIT_TEST_CL_, __LINE__) : public general::UnitTest { \
    public:                                                             \
        virtual const char* getName()                                   \
        {                                                               \
            return #NAME;                                               \
        }                                                               \
                                                                        \
        virtual void test();                                            \
    } UT_UNIQUE_NAME(UNIT_TEST_OBJ_, __LINE__);                      \
                                                                        \
    void UT_UNIQUE_NAME(UNIT_TEST_CL_, __LINE__)::test()

}

#endif

