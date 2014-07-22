#pragma once

#include "cWorldStorage.h"

#include <string>

class cChunkStreamer
{
public:
	cChunkStreamer(cWorldStorage * _ws);
	~cChunkStreamer();

	bool Check(int x, int y) const;
	void Load(int x, int y) const;
	void Save(int x, int y) const;

private:
	std::string BuildFileName(int x, int y) const;

	cWorldStorage * ws;

};