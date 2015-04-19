#pragma once

class appState
{
public:
    appState() : Dead(false) {}
    virtual ~appState() {}

    virtual bool initState() = 0;           //вызываетс€, когда стейт инициализируетс€
    virtual bool update(double dt) = 0;     //вызываетс€ дл€ обновлени€ состо€ний подконтрольных стейту
    virtual bool render(double dt) = 0;     //рендер
    virtual void onKill() = 0;              //вызываетс€ при убийстве стейта
    virtual void onSuspend() = 0;           //вызываетс€ при смене стейта на другой
    virtual void onResume() = 0;            //вызываетс€ при возвращении к этому стейту

    bool isDead() const { return Dead; }
    void kill() { Dead = true; }

private:
    bool Dead;

};
