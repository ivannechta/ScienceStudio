#pragma once
#include "header.h"
class TCore;
class TParceCmd
{
private:	
	int PassName(char* text, int i);
	bool isNameChar(unsigned char ch);
	bool isDigit(char ch);
public:
	TCore* Core;
	int PassNameSpace(char* text, int i);
	int PassFuncName(char* text, int i);
	bool ParceFunctionCall(char* text, int i);
	bool ParceParams(char* text, int i);
	void Parce();

	TParceCmd(TCore* _core) {
		Core = _core;
	}
};