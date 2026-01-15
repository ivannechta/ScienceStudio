#pragma once
/*There are just Fields with simple types, because this header will be included in modules*/
class TContext {
public:
	void* GL_surface; //for plot and 3D modeling
	char* FileName; //result output file or console if Filename is NULL
	void* Vars; // Variable list with values, search methods
	void* Arguments; //console arguments for parsing and function call
	void* Settings; // list of loaded modules and other settins
};
