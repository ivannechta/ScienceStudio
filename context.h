#pragma once
#include <stdlib.h>
#include "header.h"
#include "vars.h"
const uint16_t MAX_PARAMS = 256;
/*There are just Fields with simple types, because this header will be included in modules*/
/*struct TArguments {
	int argc;
	char** argv;
};*/
struct TArguments {
	int argc;
	TVar** argv;
};
class TContext {
	void InitArgs();
public:
	void* GL_surface; //for plot and 3D modeling
	char* FileName; //result output file or console if Filename is NULL
	void* Vars; // Variable list with values, search methods
	void* Arguments; //console arguments for parsing and function call
	void* Settings; // list of loaded modules and other settins
	TContext() {
		FileName = NULL;
		GL_surface = Vars = Settings = NULL;
		Arguments = (void*)new TArguments;
		//InitArgs();
	}
	void AddArgs(char * _param);
	void ClearArgs();
};
