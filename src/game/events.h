#pragma once

enum GameEvent
{
	//World events
	ChunkUnload,			//on chunk unloading or deleting from world
	ChunkLoad,				//on chunk loading or creating
	ChunkChanged,			//on chunk changing by player or another thing

	//Physics events
	PlayerCollidesWithItem,	//on player standing on item

	//Items events
	PlayerPickItem,			//on player picking item

};