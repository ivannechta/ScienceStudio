#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
enum EVAR_TYPE {
	EVAR_TYPE_DOUBLE,
	EVAR_TYPE_FUNC,
	EVAR_TYPE_STRING,
	EVAR_TYPE_ARRAY,
};
class TVar
{
public:
	void* Value;
	char* Name;
	char* Other; //namespace of function if need
	enum EVAR_TYPE VarType;	
	int TensorSize;  //number is 1, for func - argument count, Array - high dimention, String - strlen+1('\0').
	void* CloneMem(void* _addr, int _size);
	TVar* CloneTVar(char* _NewName);
	//void SetValue(void* _value, int* _dimen);
	//void* GetValue(int* _dimen);
	//void GetScalarValue();

	TVar(char* _name, void* _value, uint32_t _ByteSize) { //_value is a pointer to object (func address, var)		
		if (_name == NULL) { 
			Name = NULL; 
		} else {
			Name = (char*)CloneMem(_name, (int)strlen(_name) + 1);
		}
		Value = CloneMem(_value, _ByteSize);		
		Other = NULL;
		//VarType = EVAR_TYPE_DOUBLE;
	}

	~TVar() {
		delete Name;		

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
	v->TensorSize = 3;
	int dim[] = {4,5,6};
	float *f = (float*)(v->GetValue(dim));
	printf("%f\n",*f);
	return v;
}

	TTableVars TableVars;
	TableVars.Add(var((char*)"Flag", 1));

*/

