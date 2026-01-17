#pragma once
#include "header.h"
#include "context.h"
#include "settings.h"
#include <windows.h>
#include "parcecmd.h"
const uint16_t ConsoleCommandSize = 1000;
const uint16_t OUTPUT_TEXT_MAX = 500; // Console do not shows long text
DWORD WINAPI Console_Thread(void* _core);
extern TContext Context;
class TParceCMD;
class TCore {
private:
	TParceCmd* CMD;
public:
	TContext* Context_lnk;
	TSettings* Settings;
	char* ConsoleInput;		// User entered command
	void	Prompt();
	void	Execute();			// Parse and Execute console command
	void	ConsoleOutput(char* buffer, uint32_t size);	// Print to console or file

public:
	TCore() {
		Context_lnk = &Context;
		ConsoleInput = new char[ConsoleCommandSize + 1];
		Settings = new TSettings();
		CMD = new TParceCmd(this);
	}
	~TCore() {
		delete ConsoleInput;
		delete Settings;
		delete CMD;
	}
};
