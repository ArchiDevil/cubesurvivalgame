#include "Console.h"
#include "cInventory.h"
#include <GraphicsEngine/ShiftEngine.h>

#include <sstream>

Console::Console(size_t screenWidth, size_t screenHeight)
	: screenWidth(screenWidth)
	, screenHeight(screenHeight)
	, visibility(false)
{
}

Console::~Console()
{
}

void Console::HandleCommand()
{
	auto tokens = Tokenize(inputBuffer);
	inputBuffer.clear();
	if(tokens.empty())
		return;

	std::string commandName = tokens[0];
	auto * pGame = LostIsland::GetGamePtr();

	if (commandName == "spawn_entity")
	{
		if (tokens.size() != 5)
		{
			MainLog.Error("Wrong arguments: spawn_entity [entity_name] [pos_x] [pos_y] [pos_z]");
			return;
		}

		float x_pos = std::stof(tokens[2]);
		float y_pos = std::stof(tokens[3]);
		float z_pos = std::stof(tokens[4]);

		pGame->EntityMgr->CreateEntity(Vector3F(x_pos, y_pos, z_pos), tokens[1]);
	}
	else if(commandName == "add_item")
	{
		if (tokens.size() != 2)
		{
			MainLog.Error("Wrong arguments: add_item [item_name]");
			return;
		}

		unsigned int count = std::stoul(tokens[2]);

		pGame->Player->GetInventoryPtr()->AddItem(pGame->ItemMgr->GetItemId(tokens[1]));
	}
	else if(commandName == "go")
	{
		if(tokens.size() != 3)
		{
			MainLog.Error("Wrong argument: go [pos_x] [pos_y]");
			return;
		}

		float x_pos = std::stof(tokens[1]);
		float y_pos = std::stof(tokens[2]);

		if(pGame->Player)
			pGame->Player->Go(Vector2F(x_pos, y_pos));
	}
	else
	{
		MainLog.Error("Unable to recognize command: " + commandName);
		return;
	}
}

void Console::Draw()
{
	if (!visibility)
		return;

	auto * pFontManager = ShiftEngine::GetContextManager()->GetFontManager();
	auto currentFont = pFontManager->GetCurrentFontName();
	pFontManager->SetFont(L"2");
	pFontManager->DrawTextTL(">> " + inputBuffer, 10, screenHeight - 20);
	pFontManager->SetFont(currentFont);
}

std::vector<std::string> Console::Tokenize(const std::string & input) const
{
	std::stringstream ss;
	std::vector<std::string> out;
	ss << input;

	while (!ss.eof())
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
	case IE_SpecialKey:
		if (event.key == SK_BACKSPACE)
		{
			if (inputBuffer.size())
			{
				inputBuffer.pop_back();
			}
		}

		if (event.key == SK_ENTER)
		{
			HandleCommand();
			visibility = false;
		}

		return true;
	default:
		return false;
	}
}

void Console::SetVisibility(bool val)
{
	visibility = val;
}

bool Console::GetVisibility() const
{
	return visibility;
}

void Console::SetScreenWidth(size_t width)
{
	screenWidth = width;
}

void Console::SetScreenHeight(size_t height)
{
	screenHeight = height;
}
