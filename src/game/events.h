#pragma once

enum GameEventTypes
{
	//Entity events
	PlayerUsesEntity,
};

struct IGameEvent
{
	virtual GameEventTypes GetType() const = 0;
};

struct UseEvent : public IGameEvent
{
	GameEventTypes GetType() const override
	{
		return PlayerUsesEntity;
	}
};
