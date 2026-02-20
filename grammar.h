#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tablevars.h"
#include "context.h"
const int NOT_A_SIGN = 0;
const int BAD_NAME = -1;
const int BAD_FLOAT = -1;

struct TStack {
	char* data;
	struct TStack* next;
};
struct TVarStack {
	TVar* data;
	struct TVarStack* next;
};
struct TExpressionResult {
	TVar* Value;
	TStack* stk;
	bool error;
};
typedef TVar* (*ModuleFuncType)();

class TGrammar
{
private:	
	TTableVars* TableVars;
	struct TExpressionResult ExpressionResult;
	TContext* Context;

public:
	//TODO: Unary minus before name
	struct TStack* Stack;							// poliz result
	struct TStack* Stack_tmp;						// poliz stack for temporary saving
	struct TVarStack* VarStack;
	char* pop(struct TStack** _stack);
	void push(struct TStack** _stack, char* _data);
	TVar* pop(struct TVarStack** _stack);
	void push(struct TVarStack** _stack, TVar* _data);
	void ShowStack(struct TStack* _stack);

	int GetSignPriority(char operation);
	int UReadInt(char* _expression, int _pos);
	int UReadFloat(char* _expression, int _pos);

	int ReadFloat(char* _expression, int _pos);
	int ReadName(char* _expression, int _pos);
	char* GetNewStr(char* _expression, int _start, int _end); // Allocate new str and initiate it

	bool isNameChar(unsigned char ch);
	bool isDigit(char ch);

	bool PolizArithm(char* _expression);
	void CalcExpr(char* _expression);
	double ApplySign(double _a, char znak, double _b);
	TVar* ApplySign(TVar* _a, char znak, TVar* _b);

	void CalcOneStep_process_var_const(TStack* stk, TExpressionResult* Result); //aux for refactoring
	void CalcOneStep_process_var_double(TVar* var, TStack* stk, TExpressionResult* Result); //aux for refactoring
	void CalcOneStep_process_var_func(TVar* var, TStack* stk, TExpressionResult* Result); //aux for refactoring
	void CalcOneStep_process_var_array(TVar* var, TStack* stk, TExpressionResult* Result); //aux for refactoring
	void CalcOneStep_process_sign_array(TStack* stk, TExpressionResult* Result); //aux for refactoring

	TExpressionResult CalcOneStep(TStack* stk);

	TGrammar(TTableVars* _table, TContext* _context) {
		Stack = Stack_tmp = NULL;
		VarStack = NULL;
		ExpressionResult = { 0, NULL };
		TableVars = _table;
		Context = _context;
	}
};
