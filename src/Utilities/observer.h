#pragma once

#include <list>

template<typename T>
class observer;

template<typename T>
class notifier
{
public:
    notifier() {}
    virtual ~notifier() {}

    void add_observer(observer<T> * obs) { observers.push_back(obs); }
    void remove_observer(observer<T> * obs) { observers.remove(obs); }
    virtual void notifyAll(const T& ev)
    {
        for (auto * object : observers)
            object->handleEvent(ev);
    }

protected:
    std::list<observer<T>*> observers;
};

template<typename T>
class observer
{
public:
    observer() : cur_notifier(nullptr) {}
    virtual ~observer() {}

    void subscribe(notifier<T> * notifier) { if (notifier) notifier->add_observer(this); }
    void unsubscribe(notifier<T> * notifier) { if (notifier) notifier->remove_observer(this); }

    virtual bool handleEvent(const T & event) = 0;

protected:
    notifier<T> * cur_notifier;
};
