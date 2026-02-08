#include "vars.h"
#include <inttypes.h>
#include<stdio.h>

void* TVar::CloneMem(void* _addr, int _size)
{
	if (!_addr) return NULL;
	void* _Object = new char[_size];
	memcpy(_Object, _addr, _size);
	return _Object;
}

TVar* TVar::CloneTVar(char* _NewName)
{
	TVar* var = NULL;

	if (VarType == EVAR_TYPE_DOUBLE) {
		double* d = new double;
		*d = *(double*)Value;
		var = new TVar(_NewName, d, sizeof(double));
		var->VarType = VarType;		
		var->TensorSize = 1;
		var->Other = NULL;
		return var;
	}

	if (VarType == EVAR_TYPE_STRING) {		
		var = new TVar(_NewName, Value, TensorSize + 1);
		var->VarType = VarType;
		var->TensorSize = 1;
		var->Other = NULL;
		return var;
	}

	if (VarType == EVAR_TYPE_FUNC) {
		var = new TVar(_NewName, Value, sizeof(void*));
		var->VarType = VarType;
		var->TensorSize = TensorSize;
		if (Other) {
			int _len = (int)strlen(Other);
			var->Other = new char[_len + 1];
			strcpy_s(var->Other, _len + 1, Other);
		} else { 
			var->Other = NULL; 
		}
		return var;
	}
	if (VarType == EVAR_TYPE_ARRAY){
		var = new TVar(_NewName, NULL, 0);
		var->VarType = VarType;
		var->TensorSize = TensorSize;
		var->Other = NULL;
		var->Value = new TVar * *[TensorSize];
		TVar** _p = (TVar**)var->Value;
		TVar** _src = (TVar**)(Value);

		for (int i = 0; i < TensorSize; i++) {
			_p[i] = _src[i]->CloneTVar(NULL);
		}		
	}
	return var;
}

/*void* TVar::GetValue(int* _dimen)
{
	if (VarType == EVAR_TYPE_FUNC) {
		return Value;
	}
	uint32_t offset = 0;
	uint32_t tmp ;
	for (int i = 0; i < TensorSize; i++) {
		if (_dimen[i] >= Tensor[i]) {
			printf("Array is out of range\n");
			return NULL;
		}
		tmp = 1;
		for (int j = TensorSize - 1; j > i ; j--) {
			tmp *= Tensor[j];			
		}
		offset += (_dimen[i] * tmp);		
	}
	printf("res=%d\n", offset);	
	return &((double*)Value)[offset];
}*/
