#include "tablevars.h"
#include <string.h>
#include <stdio.h>

void* TTableVars::CloneMem(void* _addr, int _size)
{
	void* _Object = new char[_size];
	memcpy(_Object, _addr, _size);
	return _Object;
}

TVar* TTableVars::Search(char* _name)
{
	for (int i = 0; i < TableSize; i++) {
		if (strcmp(Table[i]->Name, _name) == 0) {
			return Table[i];
		}
	}
	return nullptr;
}
int TTableVars::SearchIndex(char* _name)
{
	for (int i = 0; i < TableSize; i++) {
		if (strcmp(Table[i]->Name, _name) == 0) {
			return i;
		}
	}
	return -1;
}

void TTableVars::Add(char* _NewName, TVar* _var)
{
	TVar* var;
	var = _var->CloneTVar(_NewName);

	int i = SearchIndex(var->Name);
	if (i == -1) { //create new
		Table[TableSize] = var;
		TableSize++;
	}
	else { //Update var
		delete Table[i];
		Table[i] = var;
	}
}

void TTableVars::AddFunc(char* _name, int _paramsCount, void* _address)
{
	void* a = &_address;
	TVar* var_a = new TVar(_name, a, sizeof(a));
	var_a->Other = NULL;
	var_a->TensorSize = _paramsCount;
	var_a->VarType = EVAR_TYPE_FUNC;
	Add(_name, var_a);
}

void TTableVars::ShowTable() const
{
	for (int i = 0; i < TableSize; i++) {

		if (Table[i]->VarType == EVAR_TYPE_DOUBLE) {
			printf("variable %s = ", Table[i]->Name);
		}
		if (Table[i]->VarType == EVAR_TYPE_FUNC) {
			printf("function %s(%d args)", Table[i]->Name, Table[i]->TensorSize);
		}
		if (Table[i]->VarType == EVAR_TYPE_ARRAY) {
			printf("array %s[%d]\n", Table[i]->Name, Table[i]->TensorSize);
		}
		ShowVar((TVar*)(Table[i]));
		printf("\n");
	}
}

void TTableVars::ShowVar(TVar* _var) const {
	double* _d; char* _str;
	TVar** _p;
	//printf("(%d;%d)",_var->VarType, _var->TensorSize);
	if (_var->VarType == EVAR_TYPE_DOUBLE) {
		_d = (double*)_var->Value;
		printf("%.2f ", *_d);
		return;
	}
	if (_var->VarType == EVAR_TYPE_FUNC) {
		return;
	}
	if (_var->VarType == EVAR_TYPE_STRING) {
		_str = (char*)_var->Value;
		if (_str) {
			printf("%s ", _str);
		}
		return;
	}
	if (_var->VarType == EVAR_TYPE_ARRAY) {
		_p = (TVar**)_var->Value;
		for (int i = 0; i < _var->TensorSize; i++) {
			ShowVar(_p[i]);
			printf("\n");
		}
		return;
	}

}
