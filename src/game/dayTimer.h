#pragma once

class dayTimer
{
public:
	dayTimer(unsigned int _hours = 0, unsigned int _minutes = 0) 
	: currentTime(0.0), hours(_hours), minutes(_minutes) 
	{
	}

	void setTime(int hours, int minutes)
	{
		if(!valid(hours, minutes))
			return;

		this->hours = hours;
		this->minutes = minutes;
		currentTime = (double)hours * 60.0 + (double)minutes;
	}

	unsigned int getHours() const
	{
		return hours;
	}

	unsigned int getMinutes() const
	{
		return minutes;
	}

	void update(double deltaTime)
	{
		currentTime += deltaTime;

		if(currentTime >= (24.0f * 60.0f))
			currentTime -= 24.0f * 60.0f;

		hours = (int)(currentTime / 60) % 24;
		minutes = (int)currentTime % 60;
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
	unsigned int hours, minutes;

};