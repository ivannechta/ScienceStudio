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
	enum EVAR_TYPE VarType;
	int* Tensor;
	int TensorSize;
	//void SetValue(void* _value, int* _dimen);
	void* GetValue(int* _dimen);
	//void GetScalarValue();

	TVar(char* _name, void* _value, uint32_t _ByteSize) {
		//int _size = strlen(_name);
		Name = new char[strlen(_name) + 1];
		memcpy(Name, _name, strlen(_name) + 1);

		//Value = _value;
		Value = malloc(_ByteSize);
		if (Value && _value) {
			memcpy(Value, _value, _ByteSize);
			float* f = (float*)Value;			
		} else {
			Value = NULL;
		}
		Tensor = NULL;
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

