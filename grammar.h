#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct TStack {
	char* data;
	struct TStack* next;
};
class TGrammar
{
private:
	//struct TStack* Stack, *Stack_tmp;

public:
	//TODO: Unary minus before name
	struct TStack* Stack, * Stack_tmp;
	char* pop(struct TStack** _stack);
	void push(struct TStack** _stack, char* _data);
	void ShowStack(struct TStack* _stack);

	int Priority(char operation);
	int UReadInt(char* _expression, int _pos);
	int UReadFloat(char* _expression, int _pos);

	int ReadFloat(char* _expression, int _pos);
	int ReadName(char* _expression, int _pos);
	char* GetNewStr(char* _expression, int _start, int _end); // Allocate new str and initiate it

	bool isNameChar(unsigned char ch);
	bool isDigit(char ch);

	char* PolizArithm(char* _expression);

	TGrammar() {
		Stack = Stack_tmp = NULL;
	}
};

