#include "CppUnitTest.h"
#include "../math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace MathLib;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::Vector3F>(const MathLib::Vector3F& t)
{
	std::wostringstream s;
	s << t.x << " " << t.y << " " << t.z << "\n";
	return s.str();
}

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::mat3f>(const MathLib::mat3f& t)
{
	std::wostringstream s;
	for (int i = 0; i < 3; ++i)
		s << t[i][0] << " " << t[i][1] << " " << t[i][2] << "\n";
	return s.str();
}

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::mat4f>(const MathLib::mat4f& t)
{
	std::wostringstream s;
	for (int i = 0; i < 4; ++i)
		s << t[i][0] << " " << t[i][1] << " " << t[i][2] << " " << t[i][3] << "\n";
	return s.str();
}

bool compareFloats(const float a, const float b)
{
	return abs(a - b) < 0.0001f;
}

namespace UnitTests
{
	TEST_CLASS(MathLibMatrixTests)
	{
	public:
		TEST_METHOD(OperatorsTest)
		{
			matrix<float, 4> mat;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat[i][j] = (float)i + j;

			Assert::AreEqual(mat[3][2], 5.0f);

			matrix<float, 4> mat2;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat2[i][j] = -((float)i + j);

			mat = mat + mat2;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					Assert::AreEqual(mat[i][j], 0.0f);

			mat = mat - mat2;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);

			mat = mat + 10.0f;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					Assert::AreEqual(mat[i][j], (float)i + j + 10.0f);

			mat = mat - 10.0f;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);

			mat = mat / 1.0f;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					Assert::AreEqual(mat[i][j], (float)i + j);
		}

		TEST_METHOD(MatrixUnitTest)
		{
			matrix<float, 4> mat;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat[i][j] = (float)i + j;

			matrix<float, 4> mat2;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat2[i][j] = -((float)i + j);

			matrix<float, 3> mat3ftype;
			for (int i = 0; i < 3; i++)
				mat3ftype[i][i] = 1.0f;
			Vector3F vec = Vector3F(1.0f, 1.0f, 1.0f);
			auto newVec = mat3ftype * vec;
			for (int i = 0; i < 3; i++)
				Assert::AreEqual(vec.el[i], 1.0f);

			//transpose test
			auto newmat = mat3ftype.transpose();
			for (int i = 0; i < 3; i++)
				Assert::AreEqual(newmat[i][i], 1.0f);

			mat2 = mat2 * 0.0f;
			Assert::AreEqual(mat2[0][0], 0.0f);
			for (int i = 0; i < 4; i++)
				mat2[i][i] = 1.0f;

			auto mat3 = mat;
			mat = mat * mat2;
			Assert::IsTrue(mat == mat3);
			Assert::IsFalse(mat == mat2);
			Assert::IsTrue(mat != mat2);
			Assert::IsFalse(mat != mat3);

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat[i][j] = (float)i + j;
		}

		TEST_METHOD(SpecialFunctionsTest)
		{
			matrix<float, 4> mat;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat[i][j] = (float)i + j;

			Assert::AreEqual(mat[3][2], 5.0f);

			matrix<float, 4> mat2;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					mat2[i][j] = -((float)i + j);

			matrix<float, 4> mat3 = mat;

			matrix<float, 3> resultMat;
			resultMat[0][0] = 2.0f; resultMat[0][1] = 3.0f; resultMat[0][2] = 4.0f;
			resultMat[1][0] = 3.0f; resultMat[1][1] = 4.0f; resultMat[1][2] = 5.0f;
			resultMat[2][0] = 4.0f; resultMat[2][1] = 5.0f; resultMat[2][2] = 6.0f;
			Assert::AreEqual(resultMat, mat.minor(0, 0), L"Minor function is wrong");
			Assert::AreEqual(0.0f, matrixDeterminant(resultMat), L"Determinant is wrong");

			resultMat[0][0] = 1.0f; resultMat[0][1] = 3.0f; resultMat[0][2] = 4.0f;
			resultMat[1][0] = 2.0f; resultMat[1][1] = 4.0f; resultMat[1][2] = 5.0f;
			resultMat[2][0] = 3.0f; resultMat[2][1] = 5.0f; resultMat[2][2] = 6.0f;
			Assert::AreEqual(resultMat, mat.minor(0, 1), L"Minor function is wrong");
			Assert::AreEqual(0.0f, matrixDeterminant(resultMat), L"Determinant is wrong");

			resultMat[0][0] = 1.0f; resultMat[0][1] = 2.0f; resultMat[0][2] = 4.0f;
			resultMat[1][0] = 2.0f; resultMat[1][1] = 3.0f; resultMat[1][2] = 5.0f;
			resultMat[2][0] = 3.0f; resultMat[2][1] = 4.0f; resultMat[2][2] = 6.0f;
			Assert::AreEqual(resultMat, mat.minor(0, 2), L"Minor function is wrong");
			Assert::AreEqual(0.0f, matrixDeterminant(resultMat), L"Determinant is wrong");

			resultMat[0][0] = 1.0f; resultMat[0][1] = 2.0f; resultMat[0][2] = 3.0f;
			resultMat[1][0] = 2.0f; resultMat[1][1] = 3.0f; resultMat[1][2] = 4.0f;
			resultMat[2][0] = 3.0f; resultMat[2][1] = 4.0f; resultMat[2][2] = 5.0f;
			Assert::AreEqual(resultMat, mat.minor(0, 3), L"Minor function is wrong");
			Assert::AreEqual(0.0f, matrixDeterminant(resultMat), L"Determinant is wrong");

			auto minorMatrix = resultMat.minor(0, 0);
			Assert::AreEqual(-1.0f, matrixDeterminant(minorMatrix), L"Determinant is wrong");

			resultMat[0][0] = 0.0f; resultMat[0][1] = 2.0f; resultMat[0][2] = 3.0f;
			resultMat[1][0] = 1.0f; resultMat[1][1] = 3.0f; resultMat[1][2] = 4.0f;
			resultMat[2][0] = 3.0f; resultMat[2][1] = 5.0f; resultMat[2][2] = 6.0f;
			Assert::AreEqual(resultMat, mat.minor(2, 1), L"Minor function is wrong");
			Assert::AreEqual(0.0f, matrixDeterminant(resultMat), L"Determinant is wrong");

			matrix<float, 3> testMat;
			testMat[0][0] = 3.0f; testMat[0][1] = 0.0f; testMat[0][2] = 2.0f;
			testMat[1][0] = 2.0f; testMat[1][1] = 0.0f; testMat[1][2] = -2.0f;
			testMat[2][0] = 0.0f; testMat[2][1] = 1.0f; testMat[2][2] = 1.0f;

			resultMat[0][0] = 0.2f; resultMat[0][1] = 0.2f; resultMat[0][2] = 0.0f;
			resultMat[1][0] = -0.2f; resultMat[1][1] = 0.3f; resultMat[1][2] = 1.0f;
			resultMat[2][0] = 0.2f; resultMat[2][1] = -0.3f; resultMat[2][2] = 0.0f;

			Assert::AreEqual(resultMat, matrixInverse(testMat));
		}
	};
}
