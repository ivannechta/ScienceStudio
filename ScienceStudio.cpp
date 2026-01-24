#include <iostream>
#include "core.h"
#include "interface.h"
#include <windows.h>
#include "parcecmd.h"
#include "grammar.h"
#include "tablevars.h"

bool isRunning = true;


int main()
{	
	TCore Core;
	HANDLE HandleConsoleThread = CreateThread(NULL, 0, Console_Thread, &Core, 0, NULL);
	Sleep(100); // Wait until pipe created
	Core.Settings->ParceSettings();

	
	TGrammar g(Core.Table);
	g.CalcExpr((char*)"a=2");
	g.CalcExpr((char*)"b=3+a*5");
	Core.Table->ShowTable();
	return 0;


	SHOW("Enter commands\n");
	while (isRunning) {
		Core.Prompt();
		gets_s(Core.ConsoleInput, ConsoleCommandSize);
		Core.Execute();
	}
}