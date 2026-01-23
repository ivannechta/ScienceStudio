#pragma once
#include<stdlib.h>
#include "vars.h"
const int MAX_TABLE_VARS_SIZE = 256;

class TTableVars
{
public:
	TVar** Table;
	int TableSize;

	TVar* Search(char* _name);
	int SearchIndex(char* _name);
	void Add(TVar* _var);
	void ShowTable();

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

