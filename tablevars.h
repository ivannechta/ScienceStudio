#pragma once
#include<stdlib.h>
#include "vars.h"
const int MAX_TABLE_VARS_SIZE = 256;

class TTableVars
{
private:
	void* CloneVar(void* _addr, int _size);
public:
	TVar** Table;
	int TableSize;

	TVar* Search(char* _name);
	int SearchIndex(char* _name);
	void Add(TVar* _var);
	void AddScalar(char* _name, double _value);
	void AddFunc(char* _name, char* _namespace, int _paramsCount, void* Address);
	void ShowTable() const;



	TTableVars() {
		Table = new TVar* [MAX_TABLE_VARS_SIZE];
		for (int i = 0; i < MAX_TABLE_VARS_SIZE; i++) {
			Table[i] = NULL;
		}
		TableSize = 0;
	}
	~TTableVars() {
		for (int i = 0; i < MAX_TABLE_VARS_SIZE; i++) {
			delete Table[i];
			Table[i] = NULL;
		}
		delete[] Table;
	}
};

