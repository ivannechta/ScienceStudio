#include <iostream>
#include "core.h"
#include "interface.h"
#include <windows.h>

bool isRunning = true;
char PROMPT[] = "\n>";
char Filename[] = "k.txt";
int main()
{
    TCore Core;     
    Core.Settings->ParceSettings();
    //Core.Settings->ModuleFunction[0]();
    SHOW("Enter command\n");
    Context.FileName=Filename;
    while (isRunning) {
        printf(PROMPT);
        scanf_s("%s", Core.ConsoleInput, ConsoleCommandSize);
        Core.Execute();

        char b[] = "12342";
        Core.ConsoleOutput(b,5);
    }    
}