#pragma once
#include "header.h"
#include "context.h"
#include "settings.h"
#include <windows.h>
const uint16_t ConsoleCommandSize = 1000;
const uint16_t OUTPUT_TEXT_MAX = 500; // Console do not shows long text
DWORD WINAPI Console_Thread(void* _core);

class TCore {
private:
	TContext *Context;
	
public:	
	TSettings* Settings;
	char*	ConsoleInput;		// User entered command
	void	Prompt();
	void	Execute();			// Parse and Execute console command
	void	ConsoleOutput(char* buffer, uint32_t size);	// Print to console or file

public:
	TCore(TContext *_ctx) {
		Context = _ctx;
		ConsoleInput = new char[ConsoleCommandSize + 1];
		Settings = new TSettings();
	}
	~TCore() {
		delete ConsoleInput;
		delete Settings;
	}
};
