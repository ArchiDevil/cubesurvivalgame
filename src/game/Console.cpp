#include "Console.h"

#include "player/cInventory.h"

#include <sstream>

Console::Console(size_t screenWidth, size_t screenHeight)
	: screenWidth(screenWidth)
	, screenHeight(screenHeight)
{
}

Console::~Console()
{
}

void Console::HandleCommand()
{
	auto tokens = Tokenize(inputBuffer);
	if(tokens.empty())
		return;

	std::string commandName = tokens[0];
	auto * pGame = LostIsland::GetGamePtr();

	if(commandName == "spawn_entity")
	{
		if(tokens.size() != 2)
			MainLog.Error("Wrong arguments: spawn_entity [entity_name]");

		pGame->EntityMgr->CreateEntity(Vector3F(), tokens[1]);
	}
	if(commandName == "add_item")
	{
		if(tokens.size() != 3)
			MainLog.Error("Wrong arguments: add_item [item_name] [count]");

		unsigned int count = std::stoul(tokens[2]);

		pGame->Player->GetInventoryPtr()->AddItem(pGame->ItemMgr->GetItemId(tokens[1]));
	}
	else
	{
		MainLog.Error("Unable to recognize command: " + commandName);
		return;
	}
}

void Console::Draw()
{
	ShiftEngine::GetContextManager()->GetFontManager()->DrawTextTL(">> " + inputBuffer, 10, screenHeight - 30);
}

std::vector<std::string> Console::Tokenize(const std::string & input) const
{
	std::stringstream ss;
	std::vector<std::string> out;
	while(ss << input)
	{
		std::string buffer;
		ss >> buffer;
		out.push_back(buffer);
	}
	return out;
}

void Console::ProcessInputKey(long key)
{
	if(key != '`' && key != '~')
		inputBuffer.push_back(key);
	else
		inputBuffer.clear();
}

bool Console::handleEvent(const InputEvent & event)
{
	switch (event.type)
	{
	case IE_SystemKey:
		ProcessInputKey((char)event.key);
		return true;
	default:
		return false;
	}
}
