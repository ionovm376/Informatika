#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\user\source\repos\ConsoleApplication13\ConsoleApplication13.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestMinMax
{
    TEST_CLASS(UnitTest10)
    {
    public:
        TEST_METHOD(Test_Single)
        {
            vector<int> a = { 42 };
            auto p = minMax(a, 0, 0);
            Assert::AreEqual(42, p.first);
            Assert::AreEqual(42, p.second);
        }
        TEST_METHOD(Test_Unsorted)
        {
            vector<int> a = { 5,7,2,4,9,6 };
            auto p = minMax(a, 0, 5);
            Assert::AreEqual(2, p.first);
            Assert::AreEqual(9, p.second);
        }
    };
}