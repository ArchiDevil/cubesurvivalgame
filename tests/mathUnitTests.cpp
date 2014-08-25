#include "stdafx.h"
#include "CppUnitTest.h"

#include "some.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace MathLib;

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

		TEST_METHOD(MatrixUnitTest)
		{
			matrix<float, 4> mat;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					mat[i][j] = (float)i + j;

			Assert::AreEqual(mat[3][2], 5.0f);

			matrix<float, 4> mat2;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					mat2[i][j] = -((float)i + j);

			mat = mat + mat2;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					Assert::AreEqual(mat[i][j], 0.0f);

			mat = mat - mat2;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);

			mat = mat + 10.0f;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					Assert::AreEqual(mat[i][j], (float)i + j + 10.0f);

			mat = mat - 10.0f;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);

			mat = mat / 1.0f;
			for (int i = 0; i < 4 ; i++)
				for (int j = 0; j < 4 ; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);

			matrix<float, 3> mat3ftype;
			for (int i = 0; i < 3 ; i++)
					mat3ftype[i][i] = 1.0f;
			Vector3F vec = Vector3F(1.0f, 1.0f, 1.0f);
			auto newVec = mat3ftype * vec;
			for (int i = 0; i < 3 ; i++)
				Assert::AreEqual(vec.el[i], 1.0f);

			//transpose test
			auto newmat = mat3ftype.transpose();
			for (int i = 0; i < 3 ; i++)
				Assert::AreEqual(newmat[i][i], 1.0f);

			mat2 = mat2 * 0.0f;
			Assert::AreEqual(mat2[0][0], 0.0f);
			for (int i = 0; i < 4 ; i++)
				mat2[i][i] = 1.0f;

			auto mat3 = mat;
			mat = mat * mat2;
			Assert::IsTrue(mat == mat3);
			Assert::IsFalse(mat == mat2);
			Assert::IsTrue(mat != mat2);
			Assert::IsFalse(mat != mat3);
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
			quaternion<float> qt = quaternion<float>(Vector3F(0.0f, 0.0f, 1.0f), (float)degrad(30.0));
			quaternion<float> qt2 = quaternion<float>(Vector3F(0.0f, 1.0f, 0.0f), (float)degrad(30.0));
			matrix<float, 3> mat;
			Vector3F vec = Vector3F(1.0f, 0.0f, 0.0f);

			qt.to_matrix(mat);
			vec = mat * vec;
			Assert::AreEqual(vec.y - 0.5f < std::numeric_limits<float>::epsilon(), true);

			quaternion<float> qt_from_matrix(mat);
			matrix<float, 3> mat2;
			qt_from_matrix.to_matrix(mat2);
			Assert::IsTrue(mat == mat2);

			qt2 = qt2.normalize();

			Assert::Fail(L"Test is not implemented fully");
		}
	};
}