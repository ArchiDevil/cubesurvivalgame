#include "Console.h"
#include "Entities/GameObjectInventory.h"
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
			LOG_ERROR("Wrong arguments: spawn_entity [entity_name] [pos_x] [pos_y] [pos_z]");
			return;
		}

		float x_pos = std::stof(tokens[2]);
		float y_pos = std::stof(tokens[3]);
		float z_pos = std::stof(tokens[4]);

		pGame->EntityMgr->CreateEntity(Vector3F(x_pos, y_pos, z_pos), tokens[1]);
	}
	else if(commandName == "add_item")
	{
		if (tokens.size() != 3)
		{
			LOG_ERROR("Wrong arguments: add_item [item_name] [count]");
			return;
		}

		std::string name = tokens[1];
        unsigned int count = std::stoul(tokens[2]);

		pGame->GlobalEventHandler->onPlayerPicksItem(pGame->ItemMgr->GetItemId(name), count);
	}
	else
	{
		LOG_ERROR("Unable to recognize command: ", commandName);
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
	// TODO: check if input is invalid
	if(key != '`' && key != '~')
		inputBuffer.push_back((char)key);
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

bool Console::IsVisible() const
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
