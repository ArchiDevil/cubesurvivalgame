#include "cChunkStreamer.h"

#include <fstream>
#include <sstream>
#include <Windows.h>

cChunkStreamer::cChunkStreamer( cWorldStorage * _ws ) :
	ws(_ws)
{
	LPSECURITY_ATTRIBUTES at;
	memset(&at, 0, sizeof(LPSECURITY_ATTRIBUTES));
	CreateDirectory(L"saves", at);
}

cChunkStreamer::~cChunkStreamer() {}

bool cChunkStreamer::Check( int x, int y ) const
{
	return (bool)std::ifstream(BuildFileName(x, y));
}

void cChunkStreamer::Load( int x, int y ) const
{
	////already checked? no, sure =)
	//if(!Check(x, y))
	//	return;

	//std::ifstream input(BuildFileName(x, y), std::ios_base::binary);	//создаем экземпл€р загрузчика
	////два пути - загружать по одному кубику, либо херачить пачкой сразу
	//size_t bufferSize = ws->GetChunkHeight() * sizeof(Block);

	//char * buffer = new char[bufferSize];

	//for (unsigned int i = 0; i < ws->GetChunkWidth(); i++)
	//{
	//	for (unsigned int j = 0; j < ws->GetChunkWidth() ; j++)
	//	{
	//		input.read(buffer, bufferSize);
	//		int index = ws->GetIndex(x * ws->GetChunkWidth() + i, y * ws->GetChunkWidth() + j, 0);
	//		memcpy(ws->WorldData + index, buffer, bufferSize);
	//		//всЄ, лол, загрузка сделана
	//	}
	//}

	//delete [] buffer;
}

void cChunkStreamer::Save( int x, int y ) const
{
// 	std::ofstream output(BuildFileName(x, y), std::ios_base::binary);
// 	size_t bufferSize = ws->GetChunkHeight() * sizeof(Block);
// 
// 	char * buffer = new char[bufferSize];
// 
// 	for (int i = 0; i < ws->GetChunkWidth(); i++)
// 	{
// 		for (int j = 0; j < ws->GetChunkWidth() ; j++)
// 		{
// 			int index = ws->GetIndex(x * ws->GetChunkWidth() + i, y * ws->GetChunkWidth() + j, 0);
// 			memcpy(buffer, ws->WorldData + index, bufferSize);
// 			output.write(buffer, bufferSize);
// 		}
// 	}
// 
// 	delete [] buffer;
}

std::string cChunkStreamer::BuildFileName( int x, int y ) const
{
	std::ostringstream out;
	out << "saves\\(" << x << ") - (" << y << ").chunk";
	return out.str();
}
