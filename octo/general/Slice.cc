
#include "Slice.hh"
#include "UnitTest.hh"
#include "verify.hh"

using namespace std;
using namespace general;

DEFINE_UNIT_TEST(Slice: Construct)
{
    general::CharSlice s1;
    assertTrue(s1.empty());
    assertEqual(s1.size(), 0);

    general::CharSlice s2("hello");
    assertFalse(s2.empty());
    assertEqual(s2.size(), 5);
    assertEqual(s2[0], 'h');
    assertEqual(s2[4], 'o');

    string str("abcd");
    general::CharSlice s3(str);
    assertFalse(s3.empty());
    assertEqual(s3.size(), 4);
    assertEqual(s3[0], 'a');
    assertEqual(s3[1], 'b');
    assertEqual(s3[2], 'c');
    assertEqual(s3[3], 'd');

    string str2("xyz");
    str2[1] = 0;

    general::CharSlice s4(str2);
    assertFalse(s4.empty());
    assertEqual(s4.size(), 3);
    assertEqual(s4[0], 'x');
    assertEqual(s4[1], 0);
    assertEqual(s4[2], 'z');
}

