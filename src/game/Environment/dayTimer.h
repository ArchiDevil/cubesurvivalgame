#pragma once

class dayTimer
{
public:
    dayTimer(unsigned int hours = 0, unsigned int minutes = 0)
        : m_hours(hours)
        , m_minutes(minutes)
    {
        currentTime = (double)hours * 60.0 + (double)minutes;
    }

    void setTime(int hours, int minutes)
    {
        if (!valid(hours, minutes))
            return;

        m_hours = hours;
        m_minutes = minutes;
        currentTime = (double)hours * 60.0 + (double)minutes;
    }

    unsigned int getHours() const
    {
        return m_hours;
    }

    unsigned int getMinutes() const
    {
        return m_minutes;
    }

    void update(double deltaTime)
    {
        currentTime += deltaTime;

        if (currentTime >= (24.0f * 60.0f))
            currentTime -= 24.0f * 60.0f;

        m_hours = (int)(currentTime / 60) % 24;
        m_minutes = (int)currentTime % 60;
    }

    double getRawTime() const
    {
        return currentTime;
    }

private:
    bool valid(int hours, int minutes) const
    {
        return hours <= 23 && hours >= 0 && minutes >= 0 && minutes <= 59;
    }

    double currentTime;
    unsigned int m_hours, m_minutes;

};