#include "stdafx.h"
#include "CppUnitTest.h"

#include "some.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SimpleGUI;

namespace UnitTests
{		
	TEST_CLASS(SimpleGUIUnitTests)
	{
	public:		
		TEST_METHOD(MainListenerTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(BaseTest)
		{
			//this is a fake test needed only for base testing purposes!
			Base * pTest = new Base(nullptr, "");
			pTest->AddChildren(pBase(new Base(nullptr)));
			auto childs = pTest->GetChildrenList();
			if(childs.cbegin() == childs.cend())
				Assert::Fail(L"Add children doesn't work");
			Assert::AreEqual(pTest->CanHaveFocus(), false, L"CanHaveFocus() test");
			Assert::AreEqual(pTest->GetName().c_str(), "", L"GetName() test");
			Assert::AreEqual((void*)pTest->GetParent(), (void*)nullptr, L"GetParent() test");
			Assert::AreEqual(pTest->GetPosition(), Point(0, 0), L"GetPosition() default test");
			Assert::AreEqual(pTest->GetSize(), Point(1, 1), L"GetSize() default test");
			pTest->Hide();
			Assert::AreEqual(pTest->IsVisible(), false, L"Visibility test");
			Assert::AreEqual(pTest->IsHit(Point(0, 0)), true, L"Hit testing");
			if(pTest->OnKeyChar('0') || pTest->OnKeyDown('0') || pTest->OnKeyUp('0') ||
				pTest->OnMouseDown(0, 0, 0) || pTest->OnMouseEnter() || pTest->OnMouseLeave() ||
				pTest->OnMouseMove() || pTest->OnMouseUp(0, 0, 0))
				Assert::Fail(L"Something wrong with On** handlers");
			pTest->RemoveAllChildrens();
			Assert::AreEqual(pTest->GetChildrenList().empty(), true, L"RemoveAllChildren() test");
			delete pTest;
		}

		TEST_METHOD(CanvasTest)
		{
			Canvas * pCanvas = new Canvas();
			Button * pBut = new Button(pCanvas);
			pBut->SetSize(100, 100);
			Assert::AreEqual((void*)pCanvas->GetControlAt(50, 50), (void*)pBut, L"Fail at GetControlAt()");
			delete pCanvas;
		}

		TEST_METHOD(ListTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(TableTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(TextTest)
		{
			Canvas * pCanvas = new Canvas();
			Text * ptext = new Text(pCanvas, "Some string");
			Assert::AreEqual(ptext->GetText().c_str(), "Some string", "GetText() test");
			ptext->SetText("Lala");
			Assert::AreEqual(ptext->GetText().c_str(), "Lala", "SetText() test");
			delete pCanvas;
		}

		TEST_METHOD(TextboxTest)
		{
			Assert::Fail(L"Test is not implemented");
		}

		TEST_METHOD(ButtonTest)
		{
			Assert::Fail(L"Test is not implemented");
		}
	};
}