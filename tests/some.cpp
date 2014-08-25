#include "stdafx.h"
#include "CppUnitTest.h"

using namespace SimpleGUI;

namespace Microsoft
{ 
	namespace VisualStudio
	{ 
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Point>(const Point& t) { std::wostringstream s; s << t.x << " " << t.y << "\n"; return s.str(); }
			template<> static std::wstring ToString<Vector3F>(const Vector3F& t) { std::wostringstream s; s << t.x << " " << t.y << " " << t.z << "\n"; return s.str(); }
		}
	}
}