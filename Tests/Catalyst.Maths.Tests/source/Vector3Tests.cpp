#include "camathspch.h"
#include "CppUnitTest.h"
#include "TestToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Catalyst
{
	TEST_CLASS(Vector3Tests)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			const Vector3 vec;

			Assert::AreEqual(0.f, vec.x);
			Assert::AreEqual(0.f, vec.y);
			Assert::AreEqual(0.f, vec.z);
		}

	};
}
