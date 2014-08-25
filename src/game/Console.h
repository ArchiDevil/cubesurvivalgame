#pragma once

#include <string>

#include "game.h"

class Console
{
public:
	Console();
	~Console();

	void HandleCommand();
	void ProcessInputKey(uint32_t key);
	void Draw();

private:
	std::vector<std::string> Tokenize(const std::string & input) const;

	std::string inputBuffer;
	ShiftEngine::Sprite * background;

};
