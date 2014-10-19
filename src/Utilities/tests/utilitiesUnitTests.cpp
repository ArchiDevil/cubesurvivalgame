#include "CppUnitTest.h"

#include <Utilities/ut.h>
#include <Utilities/BlockStreamer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilTest
{		
	TEST_CLASS(UtilsTests)
	{
	public:
		TEST_METHOD(GetExtensionTest)
		{
			auto test = utils::ExtractExtension(L"some/lol.jpg");
			Assert::AreEqual(L"jpg", test.c_str());

			test = utils::ExtractExtension(L"some/lol.arx.jpg");
			Assert::AreEqual(L"jpg", test.c_str());

			test = utils::ExtractExtension(L".jpg");
			Assert::AreEqual(L"jpg", test.c_str());

			test = utils::ExtractExtension(L"sjpg");
			Assert::AreEqual(L"", test.c_str());
		}

		TEST_METHOD(BlockStreamerTest)
		{
			BlockStreamer bs;
			uint8_t data[100] = {0};
			for(uint8_t i = 0; i < 100; i++)
			{
				data[i] = i;
			}

			Assert::IsTrue(bs.AddBlock(0, 100, data), L"Unable to add block with default params");
			Assert::IsFalse(bs.AddBlock(0, 25, data), L"It's able to add block that already exists");

			bs.AddBlock(1, 100, data);
			bs.AddBlock(2, 50, data);

			Assert::IsTrue(bs.RemoveBlock(1), L"Unable to remove existing block");

			Assert::IsTrue(bs.Save(L"here.blockdata"), L"Unable to save file");

			BlockStreamer bs2;
			Assert::IsTrue(bs2.Load(L"here.blockdata"), L"Unable to load file");

			auto & blocks = bs2.GetBlocks();
			Assert::AreEqual(blocks[0]->GetId(), (uint32_t)0, L"Wrong loaded ID");
			Assert::AreEqual(blocks[1]->GetId(), (uint32_t)2, L"Wrong loaded ID");

			Assert::AreEqual(blocks[0]->GetSize(), (uint32_t)100, L"Wrong loaded size");
			Assert::AreEqual(blocks[1]->GetSize(), (uint32_t)50, L"Wrong loaded size");

			for(auto elem : blocks)
			{
				uint8_t * dt = elem->GetData();
				for(int i = 0; i < elem->GetSize(); i++)
				{
					Assert::AreEqual(dt[i], data[i], L"Wrong loaded data");
				}
			}
		}
	};
}