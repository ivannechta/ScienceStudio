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
    
    /* Reserved */
	if (!strcmp(_paramName, "FileName")) return (void*)Context.FileName;
	if (!strcmp(_paramName, "Vars")) return Context.Vars;
	if (!strcmp(_paramName, "Settings")) return Context.Settings;
	
    return NULL;
}