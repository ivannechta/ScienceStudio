#include "tablevars.h"
#include <string.h>
#include <stdio.h>

void* TTableVars::CloneVar(void* _addr, int _size)
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

void TTableVars::Add(TVar* _var)
{
    int i = SearchIndex(_var->Name);
    if (i==-1) { //create new
        Table[TableSize] = _var;
        TableSize++;
    }
    else { //Update var
        delete Table[i];
        Table[i] = _var;
    }
}

void TTableVars::AddScalar(char* _name, double _value) {
    double* a = new double;
    int* Tensor_a = new int[1]; Tensor_a[0] = 1;
    *a = _value;
    TVar* var_a = new TVar(_name, &a, sizeof(a));
    var_a->Tensor = Tensor_a;	var_a->TensorSize = 1;
    var_a->VarType = EVAR_TYPE_FLOAT;
    var_a->Value = a;
    Add(var_a);
}

void TTableVars::AddFunc(char* _name, char* _namespace, int _paramsCount, void* _address)
{
	void* a = &_address;
    int Tensor_a[] = {_paramsCount};    
    TVar* var_a = new TVar(_name, &a, sizeof(a));    
	var_a->Other = (char*)CloneVar(_namespace, strlen(_namespace) + 1);
	var_a->Tensor = (int*)CloneVar(Tensor_a, sizeof(int)); var_a->TensorSize = 1;
    var_a->VarType = EVAR_TYPE_FUNC;    
    Add(var_a);
}

void TTableVars::ShowTable() const
{
    for (int i = 0; i < TableSize; i++) {
        printf("%s ", Table[i]->Name);
        if (Table[i]->VarType == EVAR_TYPE_FLOAT) {
            printf("variable ");
        } else {
            printf("function ");
        }
        printf("Tensor [");
        for (int j = 0; j < Table[i]->TensorSize; j++) {
            printf("%d ", Table[i]->Tensor[j]);
        }
        printf("]\n");

    }
}
