#include "vars.h"
#include <inttypes.h>
#include<stdio.h>

void* TVar::CloneVar(void* _addr, int _size)
{
	void* _Object = new char[_size];
	memcpy(_Object, _addr, _size);
	return _Object;
}

TVar* TVar::Clone(char* _NewName)
{
	TVar* var = NULL;
	if (VarType == EVAR_TYPE_FLOAT) {
		double* d = new double;
		*d = *(double*)Value;
		var = new TVar(_NewName, d, sizeof(double));
		var->VarType = VarType;
		int Tensor[] = { 1 }; var->Tensor = (int*)var->CloneVar(Tensor, sizeof(int)); var->TensorSize = 1;
		Other = NULL;
	} else {
		printf("Error. You clone complex object\n");
		// TODO copy Arrays
	}
	return var;
}

void* TVar::GetValue(int* _dimen)
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
}
