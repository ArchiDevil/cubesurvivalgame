#pragma once

#include <list>

#include "GameEvents.h"
#include "INotifer.h"

class IObservable;

class IObserver
{
public:
    ~IObserver()
    {
        for (auto it = subjects.begin(); it != subjects.end(); it++)
        {
            (*it)->removeObserver(this);
        }
    }

    virtual void ProcessEvent(GameEventType ev, void * par) = 0;

private:
    std::list<IObservable*> subjects;
};
