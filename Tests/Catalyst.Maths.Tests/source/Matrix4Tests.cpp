#include "camathspch.h"
#include "CppUnitTest.h"
#include "TestToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Catalyst
{
	TEST_CLASS(Matrix4Tests)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			const Matrix4 vec;

			Assert::AreEqual(0.f, vec[0][0]);
			Assert::AreEqual(0.f, vec[0][1]);
			Assert::AreEqual(0.f, vec[0][2]);
			Assert::AreEqual(0.f, vec[0][3]);

			Assert::AreEqual(0.f, vec[1][0]);
			Assert::AreEqual(0.f, vec[1][1]);
			Assert::AreEqual(0.f, vec[1][2]);
			Assert::AreEqual(0.f, vec[1][3]);

			Assert::AreEqual(0.f, vec[2][0]);
			Assert::AreEqual(0.f, vec[2][1]);
			Assert::AreEqual(0.f, vec[2][2]);
			Assert::AreEqual(0.f, vec[2][3]);

			Assert::AreEqual(0.f, vec[3][0]);
			Assert::AreEqual(0.f, vec[3][1]);
			Assert::AreEqual(0.f, vec[3][2]);
			Assert::AreEqual(0.f, vec[3][3]);
		}

	};
}
