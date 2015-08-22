#pragma once

#include <memory>

class ICameraController
{
public:
    virtual ~ICameraController() {}
};

typedef std::unique_ptr<ICameraController> ICameraControllerPtr;
