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

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<MathLib::Vector3F>(const MathLib::Vector3F& t)
{
    std::wostringstream s;
    s << t.x << " " << t.y << " " << t.z << "\n";
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
            Canvas * pCanvas = new Canvas();
            List * pList = new List(pCanvas);
            
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

        TEST_METHOD(TextBoxTest)
        {
            Canvas * pCanvas = new Canvas();
            TextBox * pTextbox = new TextBox(pCanvas);
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

        TEST_METHOD(ProgressBarTest)
        {
            Canvas * pCanvas = new Canvas();
            ProgressBar * pBar = new ProgressBar(pCanvas);

            pBar->SetMinValue(0);
            Assert::AreEqual(0, pBar->GetMinValue(), L"SetMinValue() or GetMinValue() don't work");

            pBar->SetMaxValue(100);
            Assert::AreEqual(100, pBar->GetMaxValue(), L"SetMaxValue() or GetMaxValue() don't work");

            pBar->SetMinValue(100500);
            Assert::AreEqual(0, pBar->GetMinValue(), L"SetMinValue must not change min value if it is more than max value");

            pBar->SetMaxValue(-100500);
            Assert::AreEqual(100, pBar->GetMaxValue(), L"SetMaxValue must not change max value if it is less than min value");

            pBar->SetValue(60);
            Assert::AreEqual(60, pBar->GetValue(), L"GetValue() doesn't work");

            pBar->SetValue(-100);
            Assert::AreEqual(pBar->GetMinValue(), pBar->GetValue(), L"SetValue() set value under limits");

            pBar->SetValue(500);
            Assert::AreEqual(pBar->GetMaxValue(), pBar->GetValue(), L"SetValue() set value above limits");

            pBar->SetMinValue(0);
            pBar->SetMaxValue(100);
            pBar->SetValue(0);
            pBar->SetMinValue(50);
            Assert::AreEqual(pBar->GetMinValue(), pBar->GetValue(), L"SetMinValue() must update current value");

            pBar->SetMinValue(0);
            pBar->SetValue(100);
            pBar->SetMaxValue(50);
            Assert::AreEqual(pBar->GetMaxValue(), pBar->GetValue(), L"SetMaxValue() must update current value");

            pBar->SetMaxValue(200);
            pBar->SetPercentValue(50.0f);
            Assert::AreEqual(100, pBar->GetValue(), L"GetValue() doesn't work with percentage");
            Assert::AreEqual(50.0f, pBar->GetPercentValue(), L"GetPercentValue() doesn't work");

            pBar->SetPercentValue(150.0f);
            Assert::AreEqual(pBar->GetMaxValue(), pBar->GetValue(), L"GetValue() doesn't work with percentage");
            Assert::AreEqual(100.0f, pBar->GetPercentValue(), L"GetPercentValue() doesn't work");

            pBar->SetMinValue(0);
            pBar->SetMaxValue(100);
            pBar->SetValue(0);
            pBar->Step();
            Assert::AreEqual(1, pBar->GetValue(), L"Step() doesn't work with default value");
            pBar->Step();
            Assert::AreEqual(2, pBar->GetValue(), L"Step() doesn't work with default value twice");

            pBar->SetStepValue(10);
            Assert::AreEqual(10, pBar->GetStepValue(), L"GetStepValue() doesn't work");
            pBar->Step();
            Assert::AreEqual(12, pBar->GetValue(), L"Step() doesn't work with custom value");

            pBar->SetValue(0);
            for (int i = 0; i < 11; ++i)
                pBar->Step();
            Assert::AreEqual(100, pBar->GetValue(), L"Step() set value over limits");

            Assert::AreEqual(MathLib::Vector3F(1.0f, 1.0f, 1.0f), pBar->GetColor(), L"GetColor() doesn't work");
            pBar->SetColor({ 1.0f, 0.0f, 0.0f });
            Assert::AreEqual(MathLib::Vector3F(1.0f, 0.0f, 0.0f), pBar->GetColor(), L"SetColor() or GetColor() don't work");
        }

        TEST_METHOD(WindowTest_NOT_IMPLEMENTED)
        {
        }
    };
}
