#include <iostream>
#include "core.h"
#include "interface.h"
#include <windows.h>

bool isRunning = true;

int main()
{
    TContext Context;
    TCore Core(&Context);
    HANDLE HandleConsoleThread = CreateThread(NULL, 0, Console_Thread, &Core, 0, NULL);
    Sleep(100); // Wait until pipe created
    Core.Settings->ParceSettings();
    //Core.Settings->ModuleFunction[0]();
    SHOW("Enter commands\n");
    while (isRunning) {
        Core.Prompt();
        scanf_s("%s", Core.ConsoleInput, ConsoleCommandSize);
        Core.Execute();
    }
}