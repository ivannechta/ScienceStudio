#pragma once
#include "header.h"
#include "context.h"
const uint16_t ConsoleCommandSize = 1000;
const uint16_t OUTPUT_TEXT_MAX = 500; // Console do not shows long text
class TCore {
private:
	
	
public:	
	char*	ConsoleInput;		// User entered command
	void	Execute();			// Parse and Execute console command
	void	ConsoleOutput(char* buffer, uint32_t size);	// Print to console or file

public:
	TCore() {
		ConsoleInput = new char[ConsoleCommandSize + 1];		
	}
	~TCore() {
		delete ConsoleInput;		
	}
};
