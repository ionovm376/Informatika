#include "pch.h"
#include "CppUnitTest.h"
#include "C:\Users\user\source\repos\ConsoleApplication11\ConsoleApplication11.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestTree
{
    TEST_CLASS(UnitTest8)
    {
    public:
        TEST_METHOD(Test_ThreePositive)
        {
            TreeNode* root = new TreeNode(1);
            root->left = new TreeNode(2);
            root->right = new TreeNode(3);
            Assert::AreEqual(6, maxPathSum(root));
        }
        TEST_METHOD(Test_NegativeLeaf)
        {
            TreeNode* root = new TreeNode(-10);
            root->left = new TreeNode(9);
            root->right = new TreeNode(20);
            root->right->left = new TreeNode(15);
            root->right->right = new TreeNode(7);
            Assert::AreEqual(42, maxPathSum(root));
        }
    };
}
