#pragma once
#include "header.h"
#include "interface.h"
#include "context.h"
extern TContext Context;
/*
for backward compatibility, if we extend Context old modules becomes failure
*/
extern "C" __declspec(dllexport) void* GetParam(char* _paramName) {
	if (!strcmp(_paramName, "Arguments")) return (void*)Context.Arguments;
	if (!strcmp(_paramName, "FileName")) return (void*)Context.FileName;
	if (!strcmp(_paramName, "Vars")) return Context.Vars;
	if (!strcmp(_paramName, "Settings")) return Context.Settings;
	return NULL;
}


/* In Module */
/*
typedef void* (*Func)(char*);
void CallEXEFunction() {
    // Get the handle of the main executable
    HMODULE hExe = GetModuleHandle(NULL);
    if (hExe != NULL) {
        // Get the address of the exported function
        Func pShowMessage = (Func)GetProcAddress(hExe, "GetParam");

        if (pShowMessage != NULL) {
            // Call the function
            printf("->%p\n", pShowMessage(Arguments));
        }
        else {
            // Handle error (function not found)
            DWORD error = GetLastError();
        }
    }
    else {
        // Handle error (could not get EXE handle)
        // ...
    }
}
======================= VTY
 HANDLE pipe = CreateFile(TEXT("\\\\.\\pipe\\ScienceStudio"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);     
    char message[] = "TEST namedPipe";
    DWORD numWritten;
    WriteFile(pipe, message, (DWORD)strlen(message), &numWritten, NULL);
*/