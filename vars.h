#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
enum EVAR_TYPE {
	EVAR_TYPE_FLOAT,
	EVAR_TYPE_FUNC,
};
class TVar
{
public:
	void* Value;
	char* Name;
	char* Other; //namespace of function if need
	enum EVAR_TYPE VarType;
	int* Tensor;
	int TensorSize;
	void* CloneVar(void* _addr, int _size);
	TVar* Clone(char* _NewName);
	//void SetValue(void* _value, int* _dimen);
	void* GetValue(int* _dimen);
	//void GetScalarValue();

	TVar(char* _name, void* _value, uint32_t _ByteSize) { //_value is a pointer to object (func address, var)		
		Name = (char*)CloneVar(_name, (int)strlen(_name) + 1);
		Value = CloneVar(_value, _ByteSize);
		Tensor = NULL;
		Other = NULL;
		TensorSize = 0; VarType = EVAR_TYPE_FLOAT;
	}

	~TVar() {
		delete Name;
		delete Tensor; 
		Tensor = NULL;

		TensorSize = 0;
		delete Value;	Value = NULL;
	}
};

/*
TVar* var(char* name, int _val) {
	float* val = new float[210];
	for (int i=1000;i<1210;i++)
		val[i-1000] = (float)i;
	TVar* v = new TVar(name, val, 210*sizeof(float));
	int* tens = new int[3];
	tens[0] = 5; tens[1] = 6; tens[2] = 7;
	v->Tensor = tens;
	v->TensorSize = 3;
	int dim[] = {4,5,6};
	float *f = (float*)(v->GetValue(dim));
	printf("%f\n",*f);
	return v;
}

	TTableVars TableVars;
	TableVars.Add(var((char*)"Flag", 1));

*/

