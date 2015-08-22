#pragma once

/*
    We are using BMFont
*/

#include <string>
#include <fstream>
#include <sstream>

#include "../ITexture.h"

struct sChar
{
    //this code taken from tutorial about BMFont
    //clean 16 bytes
    unsigned short x = 0, y = 0;
    unsigned short Width = 0, Height = 0;
    short XOffset = 0;
    unsigned short YOffset = 0;
    unsigned short XAdvance = 0;
    unsigned short Page = 0;
};

class cFont
{
public:
    bool Initialize(const std::wstring & filename, ShiftEngine::ITexturePtr _tp)
    {
        tp = _tp;
        std::ifstream in;
        in.open(filename);
        if (!in || in.fail())
            return false;
        LoadFont(in);
        return true;
    }

    inline sChar * GetCharacterPtr(unsigned char code)
    {
        return &data[code];
    }

    ShiftEngine::ITexturePtr GetTexturePtr()
    {
        return tp;
    }

    unsigned short LineHeight = 0;
    unsigned short Base = 0;
    unsigned short Width = 0;
    unsigned short Height = 0;

private:
    void LoadFont(std::ifstream & stream)
    {
        std::string Line;
        std::string Read, Key, Value;
        std::size_t i;
        while (!stream.eof())
        {
            std::stringstream LineStream;
            std::getline(stream, Line);
            LineStream << Line;

            //read the line's type
            LineStream >> Read;
            if (Read == "common")
            {
                //this holds common data
                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //assign the correct value
                    Converter << Value;
                    if (Key == "lineHeight")
                        Converter >> LineHeight;
                    else if (Key == "base")
                        Converter >> Base;
                    else if (Key == "scaleW")
                        Converter >> Width;
                    else if (Key == "scaleH")
                        Converter >> Height;
                    else if (Key == "pages")
                        Converter >> Pages;
                }
            }
            else if (Read == "char")
            {
                //this is data for a specific char
                unsigned short CharID = 0;

                while (!LineStream.eof())
                {
                    std::stringstream Converter;
                    LineStream >> Read;
                    i = Read.find('=');
                    Key = Read.substr(0, i);
                    Value = Read.substr(i + 1);

                    //assign the correct value
                    Converter << Value;
                    if (Key == "id")
                        Converter >> CharID;
                    else if (Key == "x")
                        Converter >> data[CharID].x;
                    else if (Key == "y")
                        Converter >> data[CharID].y;
                    else if (Key == "width")
                        Converter >> data[CharID].Width;
                    else if (Key == "height")
                        Converter >> data[CharID].Height;
                    else if (Key == "xoffset")
                        Converter >> data[CharID].XOffset;
                    else if (Key == "yoffset")
                        Converter >> data[CharID].YOffset;
                    else if (Key == "xadvance")
                        Converter >> data[CharID].XAdvance;
                    else if (Key == "page")
                        Converter >> data[CharID].Page;
                }
            }
        }
    }


    ShiftEngine::ITexturePtr tp = nullptr;
    std::string filename = "";

    unsigned short Pages = 0;
    sChar data[256];    //for non-unicode characters

};
