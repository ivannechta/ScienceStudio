#pragma once
#include "header.h"
class TCore;
class TParceCmd
{
private:	
	int PassName(char* text, uint16_t i);
	bool isNameChar(unsigned char ch);
	bool isDigit(char ch);
public:
	TCore* Core;
	int PassNameSpace(char* text, uint16_t i);
	int PassFuncName(char* text, uint16_t i);
	bool ParceFunctionCall(char* text, uint16_t i);
	bool ParceParams(char* text, uint16_t i);
	void Parce();

	TParceCmd(TCore* _core) {
		Core = _core;
	}
};