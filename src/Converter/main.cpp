#include <iostream>
#include <fstream>
#include <string>

#include "cXConverter.h"
#include "cObjConverter.h"
#include "cBlkConverter.h"
#include "../Utilities/ut.h"

using namespace std;
using namespace utils;

int main(int argc, char* argv[])
{
	string open, save;

	if(argc != 3)
	{
		cout << "Using: Converter.exe <input_file> <output_file>" << endl;
		return 1;
	}

	open = argv[1];
	save = argv[2];

	IConverter * converter = nullptr;

	auto ext = ExtractExtension(StrToWStr(open));
	if(ext == L"x")
	{
		converter = new cXConverter();
	}
	else if(ext == L"obj")
	{
		converter = new cObjConverter();
	}
	else if(ext == L"block" || ext == L"blk")
	{
		converter = new cBlkConverter();
	}
	else
	{
		cout << "Unknown extension. Unable to convert." << endl;
		return 1;
	}

	if(!converter->Convert(StrToWStr(open), StrToWStr(save)))
	{
		cout << "Unable to convert" << endl;
		return 1;
	}

	delete converter;
	
	return 0;
}