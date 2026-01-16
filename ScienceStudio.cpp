#include <iostream>
#include "core.h"
#include "interface.h"
#include <windows.h>
#include "parcecmd.h"

bool isRunning = true;

int main()
{
    TContext Context;
    TCore Core(&Context);
    HANDLE HandleConsoleThread = CreateThread(NULL, 0, Console_Thread, &Core, 0, NULL);
    Sleep(100); // Wait until pipe created
    Core.Settings->ParceSettings();
    SHOW("Enter commands\n");
    while (isRunning) {
        Core.Prompt();
        gets_s(Core.ConsoleInput, ConsoleCommandSize);
        Core.Execute();
    }
}