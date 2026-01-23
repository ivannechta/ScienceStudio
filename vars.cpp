#include "vars.h"
#include <inttypes.h>
#include<stdio.h>

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
	return &((float*)Value)[offset];
}
