#include "tablevars.h"
#include <string.h>
#include <stdio.h>

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

void TTableVars::ShowTable()
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
