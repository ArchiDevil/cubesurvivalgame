#include "CppUnitTest.h"
#include "../math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MathLib;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::Vector3F>(const MathLib::Vector3F& t)
{ 
	std::wostringstream s; s << t.x << " " << t.y << " " << t.z << "\n"; return s.str(); 
}

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::qaFloat>(const MathLib::qaFloat& t)
{ 
	std::wostringstream s; s << t.vector.x << " " << t.vector.y << " " << t.vector.z << " " << t.w; return s.str(); 
}

namespace UnitTests
{
	TEST_CLASS(MathLibUnitTests)
	{
	public:		
		TEST_METHOD(AABBUnitTest)
		{
			AABB aabb = AABB(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(1.0f, 1.0f, 1.0f));
			Assert::AreEqual(aabb.GetVolume(), 1.0f);
			Assert::AreEqual(aabb.IntersectsAABB(AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f))), true);
			aabb.Include(AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)));
			Assert::AreEqual(aabb.bMin, Vector3F(-0.5f, -0.5f, -0.5f));
			Assert::AreEqual(aabb.bMax, Vector3F(1.0f, 1.0f, 1.0f));
		}

		TEST_METHOD(OOBBUnitTest)
		{
			//OOBB oobb = OOBB(Vector3F(10.0f, 10.0f, 10.0f));
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(Vector2UnitTest)
		{
			vec2<float> testVec(3.0f, 2.0f);
			Assert::AreEqual(3.0f, testVec.x);
			Assert::AreEqual(2.0f, testVec.y);

			testVec.x = 1.0f;
			testVec.y = 0.0f;
			Assert::AreEqual(1.0f, testVec.length());

			Assert::IsTrue(testVec == vec2<float>(1.0f, 0.0f));
			Assert::IsTrue(testVec != vec2<float>(1.0f, 1.0f));

			testVec = vec2<float>(22.2f, 0.0f);
			Assert::AreEqual(22.2f, testVec.length());

			vec2<float> secVec = vec2<float>(12.2f, -2.0f);
			testVec += secVec;
			Assert::IsTrue(testVec == vec2<float>(34.4f, -2.0f));

			testVec -= secVec;
			Assert::IsTrue(testVec == vec2<float>(22.2f, 0.0f));

			testVec *= 2.0f;
			Assert::IsTrue(testVec == vec2<float>(44.4f, 0.0f));
		}

		TEST_METHOD(Vector3UnitTest)
		{
			vec3<float> testVec(3.0f, 2.0f, -1.0f);
			Assert::AreEqual(3.0f, testVec.x);
			Assert::AreEqual(2.0f, testVec.y);
			Assert::AreEqual(-1.0f, testVec.z);

			testVec.x = 1.0f;
			testVec.y = 0.0f;
			testVec.z = 0.0f;
			Assert::AreEqual(1.0f, testVec.length());

			Assert::IsTrue(testVec == vec3<float>(1.0f, 0.0f, 0.0f));
			Assert::IsTrue(testVec != vec3<float>(1.0f, 1.0f, 0.0f));

			vec3<float> secVec = vec3<float>(12.2f, -2.0f, 51.0f);
			testVec += secVec;
			Assert::IsTrue(testVec == vec3<float>(13.2f, -2.0f, 51.0f));

			testVec -= secVec;
			Assert::IsTrue(testVec == vec3<float>(1.0f, 0.0f, 0.0f));

			testVec *= 2.0f;
			Assert::IsTrue(testVec == vec3<float>(2.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(Vector4UnitTest)
		{
			vec4<float> testVec(3.0f, 2.0f, -1.0f, -8.0f);
			Assert::AreEqual(3.0f, testVec.x);
			Assert::AreEqual(2.0f, testVec.y);
			Assert::AreEqual(-1.0f, testVec.z);
			Assert::AreEqual(-8.0f, testVec.w);

			testVec.x = 1.0f;
			testVec.y = 0.0f;
			testVec.z = 0.0f;
			testVec.w = 0.0f;
			Assert::AreEqual(1.0f, testVec.length());

			Assert::IsTrue(testVec == vec4<float>(1.0f, 0.0f, 0.0f, 0.0f));
			Assert::IsTrue(testVec != vec4<float>(1.0f, 1.0f, 0.0f, 1.0f));

			vec4<float> secVec = vec4<float>(12.2f, -2.0f, 51.0f, -168.6f);
			testVec += secVec;
			Assert::IsTrue(testVec == vec4<float>(13.2f, -2.0f, 51.0f, -168.6f));

			testVec -= secVec;
			Assert::IsTrue(testVec == vec4<float>(1.0f, 0.0f, 0.0f, 0.0f));

			testVec *= 2.0f;
			Assert::IsTrue(testVec == vec4<float>(2.0f, 0.0f, 0.0f, 0.0f));
		}

		TEST_METHOD(PlaneUnitTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(MathLibTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(IntersectionsTests)
		{
			//sphere-sphere intersection test
			bool result = SphereSphereIntersect(Vector3F(1.0f, 0.0f, 0.0f), Vector3F(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
			Assert::AreEqual(result, true);

			result = SphereSphereIntersect(Vector3F(1.0f, 2.0f, 15.0f), Vector3F(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
			Assert::AreEqual(result, false);

			//ray-bbox intersection test
			AABB bbox(Vector3F(10.0f, -15.0f, -15.0f), Vector3F(20.0f, 15.0f, 15.0f));
			Ray r(Vector3F(0.0f, 0.0f, 0.0f), Vector3F(1.0f, 0.0f, 0.0f));
			result = RayBoxIntersect(r, bbox, 0.0f, 100.0f);
			Assert::AreEqual(result, true);

			r.Direction = Vector3F(-1.0f, 0.0f, 0.0f);
			result = RayBoxIntersect(r, bbox, 0.0f, 100.0f);
			Assert::AreEqual(result, false);

			//ray-sphere intersection test
			result = RaySphereIntersect(r, Vector3F(10.0f, 0.0f, 0.0f), 1.0f);
			Assert::AreEqual(result, false);

			r.Direction = Vector3F(1.0f, 0.0f, 0.0f);
			result = RaySphereIntersect(r, Vector3F(10.0f, 0.0f, 0.0f), 1.0f);
			Assert::AreEqual(result, true);
		}

		TEST_METHOD(QuaternionTests)
		{
			// check operators
			const qaFloat qtOp1 = quaternionFromVecAngle(vec3<float>(1.0f, 0.0f, 0.0f), degrad(30.0f));
			const qaFloat qtOp2 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
			const float eps = std::numeric_limits<float>::epsilon();

			qaFloat result;

			vec3<float> vector(1.0f, 0.0f, 0.0f);
			matrix<float, 3> mat = qtOp2.to_matrix();
			vec3<float> resVector = mat * vector;
			Assert::AreEqual(vec3<float>(0.0f, 0.0f, 1.0f), resVector, L"Converting into matrix is not correct");

			qaFloat q1 = quaternionFromVecAngle(vec3<float>(0.0f, 0.0f, 1.0f), degrad(30.0f));
			qaFloat q2 = quaternionFromVecAngle(vec3<float>(0.0f, 0.0f, 1.0f), degrad(-30.0f));
			q1 = q1.conjugate();
			Assert::AreEqual(q1, q2, L"Conjugation is not valid");

			resVector = vector * qtOp2;
			Assert::AreEqual(vec3<float>(0.0f, 0.0f, -1.0f), resVector, L"Transforming of vector is not correct");

			mat = qtOp1.to_matrix();
			result = quaternionFromMatrix(mat);
			vec3<float> differenceVector = result.vector - qtOp1.vector;
			if (differenceVector.x > eps
				|| differenceVector.y > eps
				|| differenceVector.z > eps)
				Assert::Fail(L"Matrix transformations is not valid");

			const qaFloat qtOpPlus1 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
			const qaFloat qtOpPlus2 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(0.0f));
			const qaFloat divQtOpPlus = qtOpPlus1 + qtOpPlus2;
			resVector = vector * divQtOpPlus;
			float sin_angle = sin(degrad(45.0f));
			if (resVector.x - sin_angle > eps
				|| -resVector.z - sin_angle > eps)
				Assert::Fail(L"Plus operator is incorrect");

			const qaFloat divQtOpMinus = qtOpPlus1 - qtOpPlus2;
			resVector = vector * divQtOpMinus;
			if (resVector.x - sin_angle > eps
				|| -resVector.z - sin_angle > eps)
				Assert::Fail(L"Minus operator is incorrect");

			const qaFloat qtOpMult1 = quaternionFromVecAngle(vec3<float>(0.0f, 1.0f, 0.0f), degrad(90.0f));
			const qaFloat qtOpMult2 = quaternionFromVecAngle(vec3<float>(1.0f, 0.0f, 0.0f), degrad(90.0f));
			resVector = vector * (qtOpMult1 * qtOpMult2);
			if (resVector.z + 1.0f > eps)
				Assert::Fail(L"Mult operator is incorrect");
		}
	};
}
