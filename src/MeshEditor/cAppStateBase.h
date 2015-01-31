#pragma once

class appState
{
public:
    appState() : Dead(false) {}
    virtual ~appState() = default;

    virtual bool initState() = 0;			//вызывается, когда стейт инициализируется
    virtual bool update(double dt) = 0;		//вызывается для обновления состояний подконтрольных стейту
    virtual bool render(double dt) = 0;		//рендер
    virtual void onKill() = 0;				//вызывается при убийстве стейта
    virtual void onSuspend() = 0;			//вызывается при смене стейта на другой
    virtual void onResume() = 0;			//вызывается при возвращении к этому стейту

    bool isDead() const { return Dead; }
    void kill() { Dead = true; }

private:
    bool Dead;

};
