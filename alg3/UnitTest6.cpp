#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "C:\Users\user\source\repos\ConsoleApplication8\ConsoleApplication8.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(CombSortTests)
    {
    public:

        TEST_METHOD(TestCombSort_RegularArray)
        {
            std::vector<int> arr = { 64,34,25,12,22,11,90 };
            std::vector<int> expected = { 11,12,22,25,34,64,90 };

            combSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestCombSort_AlreadySorted)
        {
            std::vector<int> arr = { 1,2,3,4,5,6,7,8,9,10 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            combSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestCombSort_ReverseSorted)
        {
            std::vector<int> arr = { 10,9,8,7,6,5,4,3,2,1 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            combSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestCombSort_AllEqual)
        {
            std::vector<int> arr = { 5,5,5,5,5,5 };
            std::vector<int> expected = { 5,5,5,5,5,5 };

            combSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestCombSort_SingleElement)
        {
            std::vector<int> arr = { 42 };
            std::vector<int> expected = { 42 };

            combSort(arr);

            Assert::AreEqual(expected[0], arr[0]);
        }

        TEST_METHOD(TestCombSort_EmptyArray)
        {
            std::vector<int> arr = {};

            combSort(arr);

            Assert::AreEqual((size_t)0, arr.size());
        }

        TEST_METHOD(TestCombSort_TwoElements)
        {
            std::vector<int> arr = { 2,1 };
            std::vector<int> expected = { 1,2 };

            combSort(arr);

            Assert::AreEqual(expected[0], arr[0]);
            Assert::AreEqual(expected[1], arr[1]);
        }
    };

    TEST_CLASS(ShellSortTests)
    {
    public:

        TEST_METHOD(TestShellSort_RegularArray)
        {
            std::vector<int> arr = { 64,34,25,12,22,11,90 };
            std::vector<int> expected = { 11,12,22,25,34,64,90 };

            shellSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestShellSort_AlreadySorted)
        {
            std::vector<int> arr = { 1,2,3,4,5,6,7,8,9,10 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            shellSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestShellSort_ReverseSorted)
        {
            std::vector<int> arr = { 10,9,8,7,6,5,4,3,2,1 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            shellSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestShellSort_AllEqual)
        {
            std::vector<int> arr = { 5,5,5,5,5,5 };
            std::vector<int> expected = { 5,5,5,5,5,5 };

            shellSort(arr);

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestShellSort_SingleElement)
        {
            std::vector<int> arr = { 42 };
            std::vector<int> expected = { 42 };

            shellSort(arr);

            Assert::AreEqual(expected[0], arr[0]);
        }

        TEST_METHOD(TestShellSort_EmptyArray)
        {
            std::vector<int> arr = {};

            shellSort(arr);

            Assert::AreEqual((size_t)0, arr.size());
        }

        TEST_METHOD(TestShellSort_TwoElements)
        {
            std::vector<int> arr = { 2,1 };
            std::vector<int> expected = { 1,2 };

            shellSort(arr);

            Assert::AreEqual(expected[0], arr[0]);
            Assert::AreEqual(expected[1], arr[1]);
        }
    };

    TEST_CLASS(QuickSortTests)
    {
    public:

        TEST_METHOD(TestQuickSort_RegularArray)
        {
            std::vector<int> arr = { 64,34,25,12,22,11,90 };
            std::vector<int> expected = { 11,12,22,25,34,64,90 };

            if (!arr.empty()) {
                quickSort(arr, 0, (int)arr.size() - 1);
            }

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestQuickSort_AlreadySorted)
        {
            std::vector<int> arr = { 1,2,3,4,5,6,7,8,9,10 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            if (!arr.empty()) {
                quickSort(arr, 0, (int)arr.size() - 1);
            }

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestQuickSort_ReverseSorted)
        {
            std::vector<int> arr = { 10,9,8,7,6,5,4,3,2,1 };
            std::vector<int> expected = { 1,2,3,4,5,6,7,8,9,10 };

            if (!arr.empty()) {
                quickSort(arr, 0, (int)arr.size() - 1);
            }

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestQuickSort_AllEqual)
        {
            std::vector<int> arr = { 5,5,5,5,5,5 };
            std::vector<int> expected = { 5,5,5,5,5,5 };

            if (!arr.empty()) {
                quickSort(arr, 0, (int)arr.size() - 1);
            }

            for (int i = 0; i < (int)arr.size(); i++) {
                Assert::AreEqual(expected[i], arr[i]);
            }
        }

        TEST_METHOD(TestQuickSort_SingleElement)
        {
            std::vector<int> arr = { 42 };
            std::vector<int> expected = { 42 };

            if (!arr.empty()) {
                quickSort(arr, 0, 0);
            }

            Assert::AreEqual(expected[0], arr[0]);
        }

        TEST_METHOD(TestQuickSort_EmptyArray)
        {
            std::vector<int> arr = {};

            Assert::AreEqual((size_t)0, arr.size());
        }

        TEST_METHOD(TestQuickSort_TwoElements)
        {
            std::vector<int> arr = { 2,1 };
            std::vector<int> expected = { 1,2 };

            if (!arr.empty()) {
                quickSort(arr, 0, (int)arr.size() - 1);
            }

            Assert::AreEqual(expected[0], arr[0]);
            Assert::AreEqual(expected[1], arr[1]);
        }
    };
}