#pragma once

/*
	We are using BMFont
*/

#include <string>
#include <fstream>
#include <sstream>

#include "../D3D10Texture.h"

struct sChar
{
	sChar() 
	: x( 0 ), y( 0 ), Width( 0 ), 
	Height( 0 ), XOffset( 0 ), YOffset( 0 ),
	XAdvance( 0 ), Page( 0 )
	{
	}

	//this code taken from tutorial about BMFont
	//clean 16 bytes
	unsigned short x, y;
	unsigned short Width, Height;
	short XOffset;
	unsigned short YOffset;
	unsigned short XAdvance;
	unsigned short Page;
	
};

class cFont
{
public:
	cFont()	: LineHeight(0), Base(0), Width(0), Height(0), Pages(0) {}
	~cFont(){}

	bool Initialize(std::wstring filename, ShiftEngine::TexturePtr _tp)
	{
		tp = _tp;
		std::ifstream in;
		in.open(filename);
		if(!in || in.fail())
			return false;
		LoadFont(in);
		return true;
	}

	inline sChar * GetCharacterPtr(unsigned char code)
	{
		return &data[code];
	}

	ShiftEngine::TexturePtr GetTexturePtr()
	{
		return tp;
	}

	unsigned short LineHeight;
	unsigned short Base;
	unsigned short Width, Height;

private:
	void LoadFont(std::ifstream & stream)
	{
		std::string Line;
		std::string Read, Key, Value;
		std::size_t i;
		while( !stream.eof() )
		{
			std::stringstream LineStream;
			std::getline( stream, Line );
			LineStream << Line;

			//read the line's type
			LineStream >> Read;
			if( Read == "common" )
			{
				//this holds common data
				while( !LineStream.eof() )
				{
					std::stringstream Converter;
					LineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "lineHeight" )
						Converter >> LineHeight;
					else if( Key == "base" )
						Converter >> Base;
					else if( Key == "scaleW" )
						Converter >> Width;
					else if( Key == "scaleH" )
						Converter >> Height;
					else if( Key == "pages" )
						Converter >> Pages;
				}
			}
			else if( Read == "char" )
			{
				//this is data for a specific char
				unsigned short CharID = 0;

				while( !LineStream.eof() )
				{
					std::stringstream Converter;
					LineStream >> Read;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "id" )
						Converter >> CharID;
					else if( Key == "x" )
						Converter >> data[CharID].x;
					else if( Key == "y" )
						Converter >> data[CharID].y;
					else if( Key == "width" )
						Converter >> data[CharID].Width;
					else if( Key == "height" )
						Converter >> data[CharID].Height;
					else if( Key == "xoffset" )
						Converter >> data[CharID].XOffset;
					else if( Key == "yoffset" )
						Converter >> data[CharID].YOffset;
					else if( Key == "xadvance" )
						Converter >> data[CharID].XAdvance;
					else if( Key == "page" )
						Converter >> data[CharID].Page;
				}
			}
		}
	}


	ShiftEngine::TexturePtr tp;
	std::string filename;

	unsigned short Pages;
	sChar data[256];	//for non-unicode characters

};