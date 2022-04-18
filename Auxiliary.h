#ifndef AUXILIARY_H
#define AUXILIARY_H

//Imports are for getting time and converting various types to strings using streams

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include "IO.h"

namespace Auxiliary
{

	//Template function prototype to convert any passed type to a string for easy logging

	template <typename T1>
	std::string toString(const T1 &typeToConvert);

	//Below we are laying the ground work for managing and tracking time with the ctime library

	struct DateTime
	{
		//Struct variables to store time data in

		time_t month;
		time_t date;
		time_t year;
		time_t hour;
		time_t minute;
		time_t second;

		//Default constructor for the DateTime struct

		DateTime()
		{
			//We are using the ctime library to create a varibale of time_t type and assign current system time to it with time() function

			time_t currentTime;
			time(&currentTime);

			//Below we will convert time_t currentTime into a ctime struct called tm in order to access specific parts like date, day, etc...

			struct tm *timeInformation = localtime(&currentTime);

			//Now we will assign our time variables to hold pieces of the date stored in the struct (i.e day, date, year)

			//The +1 is because Jan starts at 0, so you can guess why that's bad, but also we must static cast to prevent out of bound (0->11 array)
			month = static_cast<time_t>(timeInformation->tm_mon) + 1; 

			date = timeInformation->tm_mday;

			//The +1900 is inserted becuase the value returned to tm_year is basically an offset from when the clock started (i.e 2022 is 122 years post 1900)
			//The casting is to keep everything as time_t
			year = static_cast<time_t>(1900) + timeInformation->tm_year;


			hour = timeInformation->tm_hour;
			minute = timeInformation->tm_min;
			second = timeInformation->tm_sec;

		}

		//Overloaded constructor for the DateTime struct (se we can set params if we want)

		DateTime(int m, int d, int y, int H, int M, int S)
			: month(m), date(d), year(y), hour(H), minute(M), second(S) {};

		//Another overloaded constructor for date use only, times are zero'd out

		DateTime(int m, int d, int y)
			: month(m), date(d), year(y), hour(0), minute(0), second(0) {};

		//The dateToString method will format the date as a string reading mm/dd/yyyy and will return it

		const std::string getDateString(const std::string &dateSeperator)
		{
			return (month < 10 ? "0" : "") + toString(month) + dateSeperator
				+ (date < 10 ? "0" : "") + toString(date) + dateSeperator
				+ toString(year);
		}

		//The timeToString method will format the time as a string reading HH:MM:SS and will return it

		const std::string getTimeString(const std::string &hmsSeperator)
		{
			return (hour < 10 ? "0" : "") + toString(hour) + hmsSeperator
				+ (minute < 10 ? "0" : "") + toString(minute) + hmsSeperator
				+ (second < 10 ? "0" : "") + toString(second);
		}

		//The tdToString will return both the time and the date reading mm/dd/yyyy HH:MM:SS

		const std::string getDTString(
			const std::string dateSeperator,
			const std::string dtSeperator, 
			const std::string hmsSeperator)
		{
			return getDateString(dateSeperator) + dtSeperator + getTimeString(hmsSeperator);
		}

	};

	//Below is the implementation for our template function protyped at the start of this namespace

	template <typename T1>
	std::string toString(const T1 &typeToConvert)
	{
		std::ostringstream outputStringStream;
		outputStringStream << typeToConvert;
		return outputStringStream.str();
	}

	//The function logError will help us log and decode what issues the keylogger is facing at runtime

	void logError(const std::string &stringToLog)
	{
		/* Need a workaround to get AppData path for errorLogPath since I'm getting compile errors
		due to the order of the header files being included. For now, I will get it from the WinAPI 
		directly, and when I seperate the .h into .h/.cpp files, I'll try to use IO::getAppDataPath() again. */
		
		//std::string errorLogPath = IO::getAppDataPath(true);
		
		std::string errorLogPath = getenv("APPDATA") + std::string("\\Microsoft\\CLR\\");
		std::string errorLogName = "ErrorLog.txt";

		//Here we are creating an output stream to a file and opening it with correct permissions
		std::ofstream errorLog;
		errorLog.open(errorLogPath + errorLogName, std::ios::app);

		//Now we are creating a date time struct object with the default constructor which will assign current time values to relevant attributes
		DateTime currentTime;

		//Then we simply call the object's string method to create a log prefix for error messages
		errorLog << "[" << currentTime.getDTString("/"," ",":") << "]" << "\t" << stringToLog << std::endl;

		errorLog.close();
	}

	void hideConsole()
	{
		//This returns a windows handle (HWND) to ShowWindow() which will in turn hide it, use SW_RESTORE to bring back!

		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	bool isConsoleVisible()
	{
		return IsWindowVisible(GetConsoleWindow());
	}

	
}

#endif // !AUXILIARY_H
