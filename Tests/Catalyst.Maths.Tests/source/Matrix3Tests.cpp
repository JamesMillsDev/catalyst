#include "camathspch.h"
#include "CppUnitTest.h"
#include "TestToString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Catalyst
{
	TEST_CLASS(Matrix3Tests)
	{
	public:
		TEST_METHOD(DefaultConstructor)
		{
			const Matrix3 actual;

			Assert::AreEqual(0.f, actual[0][0]);
			Assert::AreEqual(0.f, actual[0][1]);
			Assert::AreEqual(0.f, actual[0][2]);

			Assert::AreEqual(0.f, actual[1][0]);
			Assert::AreEqual(0.f, actual[1][1]);
			Assert::AreEqual(0.f, actual[1][2]);

			Assert::AreEqual(0.f, actual[2][0]);
			Assert::AreEqual(0.f, actual[2][1]);
			Assert::AreEqual(0.f, actual[2][2]);
		}

		TEST_METHOD(CustomConstructor)
		{
			const Matrix3 actual = { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

			Assert::AreEqual(1.f, actual[0][0]);
			Assert::AreEqual(2.f, actual[0][1]);
			Assert::AreEqual(3.f, actual[0][2]);

			Assert::AreEqual(4.f, actual[1][0]);
			Assert::AreEqual(5.f, actual[1][1]);
			Assert::AreEqual(6.f, actual[1][2]);

			Assert::AreEqual(7.f, actual[2][0]);
			Assert::AreEqual(8.f, actual[2][1]);
			Assert::AreEqual(9.f, actual[2][2]);
		}

		TEST_METHOD(Identity)
		{
			const Matrix3 expected = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
			const Matrix3 actual = Matrix3::Identity();

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(CreatePitch)
		{
			const Matrix3 expected =
			{
				1.f, 0.f, 0.f,
				0.f, -0.668648f, -0.743579f,
				0.f, 0.743579f, -0.668648f
			};

			const Matrix3 actual = Matrix3::MakePitch(CatalystMath::Degrees(3.98f));

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(CreateYaw)
		{
			const Matrix3 expected =
			{
				-0.188077f, 0.f, -0.982154f,
				0.f, 1.f, 0.f,
				0.982154f, 0.f, -0.188077f
			};

			const Matrix3 actual = Matrix3::MakeYaw(CatalystMath::Degrees(1.76f));

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(CreateRoll)
		{
			const Matrix3 expected =
			{
				-0.981005f, -0.193984f, 0.f,
				0.193984f, -0.981005f, 0.f,
				0.f, 0.f, 1.f
			};

			const Matrix3 actual = Matrix3::MakeRoll(CatalystMath::Degrees(9.62f));

			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(CreateEulerFloats)
		{
			const Matrix3 expected =
			{
				0.4119821f, 0.0587267f, 0.9092975f,
				-0.6812426f, -0.6428730f, 0.3501754f,
				0.6051275f, -0.7637182f, -0.2248451f
			};

			const float pitch = CatalystMath::Degrees(1.f);
			const float yaw = CatalystMath::Degrees(2.f);
			const float roll = CatalystMath::Degrees(3.f);

			const Matrix3 actual = Matrix3::MakeRotation(pitch, yaw, roll);

			Assert::AreEqual(expected, actual);
		}

	};
}
