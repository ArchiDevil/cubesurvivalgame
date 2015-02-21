#include "CppUnitTest.h"

#include <GUI/GUI.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace SimpleGUI;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<SimpleGUI::Point>(const SimpleGUI::Point& t)
{
    std::wostringstream s;
    s << t.x << " " << t.y << "\n";
    return s.str();
}

namespace UnitTests
{
    TEST_CLASS(SimpleGUIUnitTests)
    {
    public:
        TEST_METHOD(MainListenerTest_NOT_IMPLEMENTED)
        {
        }

        TEST_METHOD(BaseTest)
        {
            //this is a fake test needed only for base testing purposes!
            Base * pTest = new Base(nullptr, "NAME");

            pTest->AddChildren(pBase(new Base(nullptr)));
            auto childs = pTest->GetChildrenList();
            if (childs.cbegin() == childs.cend())
                Assert::Fail(L"Add children doesn't work");

            Assert::IsFalse(pTest->CanHaveFocus(), L"CanHaveFocus() test failed");
            Assert::AreEqual(pTest->GetName().c_str(), "NAME", L"GetName() test failed");
            Assert::AreEqual((void*)pTest->GetParent(), (void*)nullptr, L"GetParent() test failed");

            pTest->SetPosition(100, 100);
            Assert::AreEqual(pTest->GetPosition(), Point(100, 100), L"GetPosition() test failed");

            pTest->SetSize(100, 100);
            Assert::AreEqual(pTest->GetSize(), Point(100, 100), L"GetSize() test failed");

            pTest->Hide();
            Assert::IsFalse(pTest->IsVisible(), L"Hide test failed");
            pTest->Show();
            Assert::IsTrue(pTest->IsVisible(), L"Show test failed");

            Assert::IsTrue(pTest->IsHit(Point(120, 120)), L"Hit item failed");

            if (pTest->OnKeyChar('0') || pTest->OnKeyDown('0') || pTest->OnKeyUp('0') ||
                pTest->OnMouseDown(LButton, 0, 0) || pTest->OnMouseEnter() || pTest->OnMouseLeave() ||
                pTest->OnMouseMove({}, {}) || pTest->OnMouseUp(LButton, 0, 0))
                Assert::Fail(L"Something is wrong with On** handlers");

            pTest->RemoveAllChildrens();
            Assert::IsTrue(pTest->GetChildrenList().empty(), L"RemoveAllChildren() test failed");
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

        TEST_METHOD(ListTest_NOT_IMPLEMENTED)
        {
        }

        TEST_METHOD(TableTest_NOT_IMPLEMENTED)
        {
        }

        TEST_METHOD(TextTest)
        {
            Canvas * pCanvas = new Canvas();
            Text * pText = new Text(pCanvas, "Some string");
            Assert::AreEqual(pText->GetText().c_str(), "Some string", "GetText() test");
            pText->SetText("Lala");
            Assert::AreEqual(pText->GetText().c_str(), "Lala", "SetText() test");
            delete pCanvas;
        }

        TEST_METHOD(TextboxTest)
        {
            Canvas * pCanvas = new Canvas();
            Textbox * pTextbox = new Textbox(pCanvas);
            Assert::IsTrue(pTextbox->CanHaveFocus(), L"Textbox cannot have focus");
            pTextbox->OnKeyChar(L't');
            pTextbox->OnKeyChar(L'e');
            pTextbox->OnKeyChar(L's');
            pTextbox->OnKeyChar(L't');
            Assert::AreEqual(pTextbox->GetText(), std::wstring(L"test"), L"Failed key char event test");
            pTextbox->SetText(L"my_test");
            Assert::AreEqual(pTextbox->GetText(), std::wstring(L"my_test"), L"Failed SetText test");
            std::wstring buffText;
            pTextbox->SetOnTextChangedCallback([&](const std::wstring & newText) { buffText = newText; });
            pTextbox->OnKeyChar(L's');
            Assert::AreEqual(pTextbox->GetText(), std::wstring(L"my_tests"), L"Failed test on text changed callback");
            delete pCanvas;
        }

        TEST_METHOD(ButtonTest)
        {
            Canvas * pCanvas = new Canvas();
            Button * pButton = new Button(pCanvas);
            pButton->OnMouseEnter();
            Assert::IsTrue(pButton->IsSelected(), L"Failed selection test");
            pButton->OnMouseLeave();
            Assert::IsFalse(pButton->IsSelected(), L"Failed selection test");

            pButton->SetText("test");
            Assert::AreEqual(pButton->GetText(), std::string("test"), L"Failed SetText test");

            if (!pButton->OnMouseEnter() || !pButton->OnMouseLeave() || !pButton->OnMouseUp(LButton, 0, 0))
                Assert::Fail(L"One of the mouse events returns false");

            // implement test for SetPicture

            bool clicked = false;
            pButton->SetClickHandler([&](MouseKeys, int, int){ clicked = true; });
            pButton->OnMouseUp(LButton, 0, 0);
            Assert::IsTrue(clicked, L"Click handler doesn't work");
            delete pCanvas;
        }

        TEST_METHOD(WindowTest_NOT_IMPLEMENTED)
        {
        }
    };
}
