#include <iostream>
#include <Windows.h>

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
