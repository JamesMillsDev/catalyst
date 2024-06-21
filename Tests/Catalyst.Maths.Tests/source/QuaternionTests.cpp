#include "camathspch.h"
#include "CppUnitTest.h"
#include "TestToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Catalyst
{
	TEST_CLASS(QuaternionTests)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			const Quaternion actual;

			Assert::AreEqual(0.f, actual.x);
			Assert::AreEqual(0.f, actual.y);
			Assert::AreEqual(0.f, actual.z);
			Assert::AreEqual(0.f, actual.w);
		}

		TEST_METHOD(FloatConstructor)
		{
			const Quaternion actual = { 1.f, 2.f, 3.f, 4.f };

			Assert::AreEqual(1.f, actual.x);
			Assert::AreEqual(2.f, actual.y);
			Assert::AreEqual(3.f, actual.z);
			Assert::AreEqual(4.f, actual.w);
		}

		TEST_METHOD(EulerConstructor)
		{
			const Quaternion actual = Quaternion(Vector3{ 45.f, 80.f, 35.f });
			const Quaternion expected = { 0.1010079f, 0.6545254f, 0.4474186f, 0.6010079f };

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(RotationMatrixConstructor)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(TransformationMatrixConstructor)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(PitchYawRollConstructor)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(Identity)
		{
			const Quaternion actual = Quaternion::Identity();

			Assert::AreEqual(0.f, actual.x);
			Assert::AreEqual(0.f, actual.y);
			Assert::AreEqual(0.f, actual.z);
			Assert::AreEqual(1.f, actual.w);
		}

		TEST_METHOD(Dot)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(LookRotation)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(AxisAngle)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(FromPitchRollYawRoll)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(FromEuler)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(FromRotationMatrix)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(FromTransformationMatrix)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(GetEuler)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(SetEulerVector)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(SetEulerPitchYawRoll)
		{
			Assert::Fail(L"Not yet implemented");
		}

	};
}
