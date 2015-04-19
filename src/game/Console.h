#pragma once

#include <string>

#include "game.h"

#include <Utilities/observer.h>
#include <Utilities/InputEngine.h>

class Console : public observer < InputEvent >
{
public:
    Console();
    ~Console();

    void HandleCommand();
    void ProcessInputKey(long key);
    void Draw();

    bool handleEvent(const InputEvent & event) override;

    void SetVisibility(bool val);
    bool IsVisible() const;

private:
    std::vector<std::string> Tokenize(const std::string & input) const;

    std::string inputBuffer;
    bool visibility;

};
