#include "Console.h"

#include <sstream>

Console::Console()
{
	background = new ShiftEngine::Sprite(L"gui/background.png");
	background->SetSizeInPixels(600, 300);
}

Console::~Console()
{
}

void Console::HandleCommand()
{
	auto tokens = Tokenize(inputBuffer);

	if(tokens.empty()) return;

	std::string command = tokens[0];

	if(command == "add_crafter")
	{
	}
	else if(command == "add_producer")
	{
	}
	else
	{
		MainLog.Error("Unable to recognize command: " + command);
	}
}

void Console::ProcessInputKey( uint32_t key )
{
	if((key >= '0' && key <= '9') || (key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z'))
	{
		inputBuffer += (char)key;
	}
}

void Console::Draw()
{
	background->Draw();
	ShiftEngine::GetContextManager()->GetFontManager()->DrawTextTL(inputBuffer, 10, 10);
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
