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
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

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

		TEST_METHOD(RollPitchYawConstructor)
		{
			const Quaternion actual = Quaternion(45.f, 80.f, 35.f);
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

			Assert::AreEqual(expected, actual);
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
			const Quaternion a = { 0.458f, 0.478f, 0.447f, 0.601f };
			const Quaternion b = { 0.447f, 0.101f, 0.654f, 0.601f };

			const float expected = 0.906543f;
			const float actual = Quaternion::Dot(a, b);

			const std::wstring str = std::to_wstring(actual);

			Assert::IsTrue(CatalystMath::Compare(expected, actual), str.c_str());
		}

		TEST_METHOD(LookRotation)
		{
			Assert::Fail(L"Not yet implemented");
		}

		TEST_METHOD(AxisAngle)
		{
			const Quaternion actual = Quaternion({ -.69f, -.29f, 0.66f }, 45.f);
			const Quaternion expected = { -0.2646078f, -0.111212f, 0.2531031f, 0.9238795f };

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(FromRollPitchYaw)
		{
			const Quaternion actual = Quaternion::From(45.f, 80.f, 35.f);
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(FromEuler)
		{
			const Quaternion actual = Quaternion::From(Vector3{ 45.f, 80.f, 35.f });
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

			Assert::AreEqual(expected, actual);
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
			const Quaternion actual = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };
			const Vector3 expected = { 45.f, 80.f, 35.f };

			Assert::AreEqual(expected, actual.Euler());
		}

		TEST_METHOD(SetEulerVector)
		{
			Quaternion actual = Quaternion::Identity();
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

			actual.EulerAngles({ 45.f, 80.f, 35.f });

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(SetEulerPitchYawRoll)
		{
			Quaternion actual = Quaternion::Identity();
			const Quaternion expected = { 0.101008f, 0.654525f, -0.021780f, 0.748946f };

			actual.EulerAngles(45.f, 80.f, 35.f);

			Assert::AreEqual(expected, actual);
		}

	};
}
