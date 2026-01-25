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
	
	TGrammar g(Core.Table, Core.Context_lnk);
	g.CalcExpr((char*)"a=sum(1,2)");
	g.CalcExpr((char*)"b=1+a*5");
	Core.Table->ShowTable();

	SHOW("Enter commands\n");
	while (isRunning) {
		Core.Prompt();
		gets_s(Core.ConsoleInput, ConsoleCommandSize);
		Core.Execute();
	}
}