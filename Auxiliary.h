#ifndef AUXILIARY_H
#define AUXILIARY_H

//Imports are for getting time and converting various types to strings using streams

#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

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

		//Below is a method which will set the current date and time in the struct attributes by calling the default constructor
		
		/*const DateTime now()
		{
			return DateTime();
		}*/

		//The dateToString method will format the date as a string reading mm/dd/yyyy and will return it

		const std::string getDateString()
		{
			return (month < 10 ? "0" : "") + toString(month) + "/"
				+ (date < 10 ? "0" : "") + toString(date) + "/"
				+ toString(year);
		}

		//The timeToString method will format the time as a string reading HH:MM:SS and will return it

		const std::string getTimeString()
		{
			return (hour < 10 ? "0" : "") + toString(hour) + ":"
				+ (minute < 10 ? "0" : "") + toString(minute) + ":"
				+ (second < 10 ? "0" : "") + toString(second);
		}

		//The tdToString will return both the time and the date reading mm/dd/yyyy HH:MM:SS

		const std::string getTDString()
		{
			return getDateString() + " " + getTimeString();
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
		//Here we are creating an output stream to a file
		std::ofstream errorLog;
		errorLog.open("errorLog.txt", std::ios::app);

		//Now we are creating a date time struct object with the default constructor which will assign current time values to relevant attributes
		DateTime currentTime;

		//Then we simply call the object's string method to create a log prefix for error messages
		errorLog << "[" << currentTime.getTDString() << "]" << " " << stringToLog << std::endl;

		errorLog.close();
	}

	
}

#endif // !AUXILIARY_H
