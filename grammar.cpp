#include "grammar.h"
#include "context.h"

char* TGrammar::pop(TStack** _stack)
{
	struct TStack* tmp = *_stack;
	char* data;
	if (tmp != NULL) {
		data = tmp->data;
		tmp = tmp->next;
		delete* _stack;
		*_stack = tmp;
		return data;
	}
	return NULL;
}

TVar* TGrammar::pop(struct TVarStack** _stack) {
	struct TVarStack* tmp = *_stack;
	TVar* data;
	if (tmp != NULL) {
		data = tmp->data;
		tmp = tmp->next;
		delete* _stack;
		*_stack = tmp;
		return data;
	}
	return NULL;

}
void TGrammar::push(TStack** _stack, char* _data)
{
	struct TStack* New = new TStack;
	New->next = *_stack;
	New->data = _data;
	*_stack = New;
}

void TGrammar::push(struct TVarStack** _stack, TVar* _data) {
	struct TVarStack* New = new TVarStack;
	New->next = *_stack;
	New->data = _data;
	*_stack = New;
}

void TGrammar::ShowStack(TStack* _stack)
{
	if (_stack != 0) {
		ShowStack(_stack->next);
		printf("%s ", _stack->data);
	}
}

int TGrammar::GetSignPriority(char operation) // sign priority or NULL if it is not a sign
{
	switch (operation) {
	case '(':
	case ')':
	case '[':
	case ']':
		return 6;
		//case '^':
	//        return 5;
	case '*':
	case '/':
		return 4;
	case '+':
	case '-':
		return 3;
	case ',':
		return 2;
	case '=':
		return 1;

	default:
		return 0;
	};
}

int TGrammar::UReadInt(char* _expression, int _pos) {
	int i = _pos;
	while ((_expression[i] != 0) &&
		(_expression[i] >= '0') &&
		(_expression[i] <= '9'))
	{
		i++;
	}
	if (i == _pos) {
		return -1;
	}
	else {
		return i - 1; //position of the last digit
	}
}
int TGrammar::UReadFloat(char* _expression, int _pos)
{
	if (_expression[_pos] == 0) {
		return -1;
	}
	int i = UReadInt(_expression, _pos);
	if (i == -1) return -1;
	i++;
	if (_expression[i] == '.') { //Frack part
		i++;
		i = UReadInt(_expression, i);
		if (i == -1) return -1;
		return i;
	}
	else {
		return i - 1;
	}

}
int TGrammar::ReadFloat(char* _expression, int _pos)
{
	if (_expression[_pos] == 0) {
		return -1;
	}
	int i = _pos;
	if (_expression[_pos] == '-') {
		i++;
	}

	i = UReadFloat(_expression, i);
	return i;
}
bool TGrammar::isNameChar(unsigned char ch) {
	if (isDigit(ch)) return true;
	if (((ch >= 'a') && (ch <= 'z')) ||
		((ch >= 'A') && (ch <= 'Z')) ||
		(ch == '.')) return true;
	return false;
}
bool TGrammar::isDigit(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}

int TGrammar::ReadName(char* _expression, int _pos)
{
	int i = _pos;
	if ((_expression[i] == 0) || isDigit(_expression[i])) {
		return -1;
	}
	while ((_expression[i] != 0) &&
		(isNameChar(_expression[i])))
	{
		i++;
	}
	if (i == _pos) {
		return -1;
	}

	return i - 1;
}
char* TGrammar::GetNewStr(char* _expression, int _start, int _end)
{
	char* str = new char[_end - _start + 2];
	str[_end - _start + 1] = 0;
	for (int i = _start; i <= _end; i++) {
		str[i - _start] = _expression[i];
	}
	return str;
}
bool TGrammar::PolizArithm(char* _expression)
{
	TVar* var = NULL;
	int i = 0;
	int j = 0;
	int size = (int)strlen(_expression);
	char* stk_znak = NULL;
	char* str; // part of the expression with name or float
	bool StartLexemma = true; // unary minus possible, but after name just arithmetic minus appears
	while (i < size) {
		if (((j = ReadName(_expression, i)) != -1) ||   // is it Name or const?
			(StartLexemma && (j = ReadFloat(_expression, i)) != -1) ||
			(!StartLexemma && (j = UReadFloat(_expression, i)) != -1))
		{
			StartLexemma = false;
			str = GetNewStr(_expression, i, j);
			var = TableVars->Search(str);

			if ((j + 1 < size) && (_expression[j + 1] == '(')) {
				//found function in expression. It should be known
				if (!var) {
					printf("function '%s' was not found\n", str);
					return false;
				}
			}
			if (var && //name of function and array should be first: 'y [1,2] array ='
				(var->VarType == EVAR_TYPE_FUNC ||
					var->VarType == EVAR_TYPE_ARRAY)) {
				push(&Stack_tmp, str);
			}
			else { // it is name of a simple variable
				push(&Stack, str);
			}
			i = j + 1;
			continue;
		}
		//--------------- ZNAK----------------------
		char* znak = new char[2];
		znak[0] = _expression[i];
		znak[1] = 0;
		if (GetSignPriority(znak[0]) != 0) // it is a operation symbol
		{
			StartLexemma = false;
			if (Stack_tmp == NULL) {
				push(&Stack_tmp, znak);
			}
			else {
				/* braces processed in a special way */
				if ((znak[0] == '(') ||
					(znak[0] == '['))

				{
					push(&Stack_tmp, znak);
					if (znak[0] == '[') { push(&Stack, znak); }
					i++;
					continue;
				}

				if ((znak[0] == ')') || (znak[0] == ']') ||
					(znak[0] == ',')) // function delimiter
				{
					while ((Stack_tmp) &&
						(Stack_tmp->data[0] != '(') &&
						(Stack_tmp->data[0] != '['))
					{
						stk_znak = pop(&Stack_tmp);
						if (stk_znak) {
							push(&Stack, stk_znak);
						}
					}
					if (znak[0] != ',') { // we stay in function or array
						pop(&Stack_tmp); // pop '('
						if (znak[0] == ']') { push(&Stack, znak); }
					}
					var = TableVars->Search(Stack_tmp->data);
					// next in stack func or array? they processed in a special way
					if (var &&
						((var->VarType == EVAR_TYPE_FUNC) ||
							(var->VarType == EVAR_TYPE_ARRAY)))
					{
						//if (strcmp(Stack_tmp->data, "func") == 0) { // Fix checking function by VAR/func search
						stk_znak = pop(&Stack_tmp); // pop function name
						if (stk_znak) {
							push(&Stack, stk_znak);
						}
					}
					i++;
					continue;
				}
				else {
					// we observe new znak, while in stack znak with lower priority move to poliz_Result
					while ((Stack_tmp) &&
						(GetSignPriority(znak[0]) <= GetSignPriority(Stack_tmp->data[0])) &&
						(Stack_tmp->data[0] != '(') &&
						(Stack_tmp->data[0] != '[')
						)
					{
						stk_znak = pop(&Stack_tmp);
						push(&Stack, stk_znak);
					}
				}
				push(&Stack_tmp, znak);
			}
			i++;
			continue;
		}
	}
	// move all remains to result
	while (Stack_tmp) {
		stk_znak = pop(&Stack_tmp);
		if (stk_znak) {
			push(&Stack, stk_znak);
		}
	}
	//ShowStack(Stack);
	return true;
}

void TGrammar::CalcExpr(char* _expression) {
	if (PolizArithm(_expression)) {
		TExpressionResult res = CalcOneStep(Stack);
		if (res.Value) {
			TableVars->ShowVar(res.Value);
		}
	}
	while (Stack) { // Clear stack for future using
		pop(&Stack);
	}
	while (Stack_tmp) { // Clear stack for future using
		pop(&Stack_tmp);
	}
	printf("\n");
}

#pragma message( "TODO: Depricated ")
double TGrammar::ApplySign(double _a, char znak, double _b) {
	switch (znak)
	{
	case '+': return _a + _b;
	case '-': return _a - _b;
	case '*': return _a * _b;
	case '/': return _a / _b;
	default:
		printf("Error in ApplyZnak\n");
		return 0;
	}
}
TVar* TGrammar::ApplySign(TVar* _a, char znak, TVar* _b) {
	TVar* var;// = new TVar("ApplySign", );
	double* d = new double;
	switch (znak)
	{
	case '+':
		if ((_a->VarType == EVAR_TYPE_DOUBLE) &&
			(_b->VarType == EVAR_TYPE_DOUBLE)) {
			*d = *(double*)_a->Value + *(double*)_b->Value;
			var = new TVar((char*)"ApplySign", d, sizeof(double));
			var->TensorSize = 1;
		}
		else { return NULL; }
		return var;
	case '-':
		if ((_a->VarType == EVAR_TYPE_DOUBLE) &&
			(_b->VarType == EVAR_TYPE_DOUBLE)) {
			*d = *(double*)_a->Value - *(double*)_b->Value;
			var = new TVar((char*)"ApplySign", d, sizeof(double));
			var->TensorSize = 1;
		}
		else { return NULL; }
		return var;
	case '*':
		if ((_a->VarType == EVAR_TYPE_DOUBLE) &&
			(_b->VarType == EVAR_TYPE_DOUBLE)) {
			*d = *(double*)_a->Value * *(double*)_b->Value;
			var = new TVar((char*)"ApplySign", d, sizeof(double));
			var->TensorSize = 1;
		}
		else { return NULL; }
		return var;
	case '/':
		if ((_a->VarType == EVAR_TYPE_DOUBLE) &&
			(_b->VarType == EVAR_TYPE_DOUBLE)) {
			*d = *(double*)_a->Value / *(double*)_b->Value;
			var = new TVar((char*)"ApplySign", d, sizeof(double));
			var->TensorSize = 1;
		}
		else { return NULL; }
		return var;
	default:
		printf("Error in ApplyZnak\n");
		return 0;
	}
}
void TGrammar::CalcOneStep_process_var_const(TStack* stk, TExpressionResult* Result) {
	TVar* var;
	double* _d = new double;
	*_d = atof(stk->data);
	var = new TVar((char*)"Num", _d, sizeof(double));
	var->TensorSize = 1;
	var->Other = NULL;
	Result->Value = var;
	Result->stk = stk->next;
}

void TGrammar::CalcOneStep_process_var_double(TVar* var, TStack* stk, TExpressionResult* Result)
{
	Result->Value = var->CloneTVar((char*)"var");
	Result->stk = stk->next;
}

void TGrammar::CalcOneStep_process_var_func(TVar* var, TStack* stk, TExpressionResult* Result)
{
	ModuleFuncType FunctionCaller;
	struct TArguments* Args = (TArguments*)Context->Arguments; // create args array	
	TExpressionResult FuncRes;
	Args->argc = var->TensorSize; // argument counts
	Args->argv = new TVar * [Args->argc];
	FuncRes.stk = stk->next;
	for (int i = 0; i < Args->argc; i++) {
		FuncRes = CalcOneStep(FuncRes.stk); // Calc every argument
		if (!FuncRes.Value) {
			*Result = FuncRes;
			return;
		}
		// put calculated argument in structure for transmitt to dll
		Args->argv[i] = FuncRes.Value->CloneTVar((char*)"funcArg");
	}
	// call func & return result
	FunctionCaller = (ModuleFuncType)(*(ModuleFuncType*)(var->Value));
	TVar* FuncResult = FunctionCaller();
	TableVars->ShowVar(FuncResult);

	// TODO check what type was returned: double or string or array Name of Tvar contains a type
	Result->Value = FuncResult;
	Result->stk = FuncRes.stk;
	// delete arguments form memory after func call(free)
	for (int i = 0; i < Args->argc; i++) {
		delete Args->argv[i];
	}
	// TODO delete FuncResult.Value;
}

void TGrammar::CalcOneStep_process_var_array(TVar* var, TStack* stk, TExpressionResult* Result, TVar* assignVar)
{
	// try to find indexes of array var[i,j]
	TStack* _p = stk->next;
	_p = _p->next;  // pass ']'
	TExpressionResult ArrayRes;
	ArrayRes.stk = _p;
	TVar* ArrayArg = NULL;
	struct TArguments* BlockArrayArgs = new TArguments; //set of array indexes (in arr[i,j,k] it is a set(k,j,i)
	BlockArrayArgs->argc = 0;
	while (_p->data[0] != '[') { // how many indexes in array?
		BlockArrayArgs->argc++;
		_p = _p->next;
	}
	BlockArrayArgs->argv = new TVar * [BlockArrayArgs->argc];
	for (int i = BlockArrayArgs->argc - 1; i >= 0; i--) { // reverse order of indexes
		ArrayRes = CalcOneStep(ArrayRes.stk); // Calc every index
		if (!ArrayRes.Value) {
			*Result = ArrayRes;
			return;
		}
		ArrayArg = ArrayRes.Value->CloneTVar((char*)"ArrayArg");
		BlockArrayArgs->argv[i] = ArrayArg;
	}
	ArrayRes.stk = ArrayRes.stk->next; //pass '['
	// now we have indexes in BlockArrayArgs, so we try to get Value of array[i,j,...]
	int index = -1;
	TVar* _tmp = var;	
	TVar** _tmp0 = (TVar**)_tmp->Value; // just init for assignVar
	for (int i = 0; i < BlockArrayArgs->argc; i++) {
		index = (int)(*((double*)(BlockArrayArgs->argv[i]->Value)));
		if ((_tmp->TensorSize > index) && (_tmp->VarType == EVAR_TYPE_ARRAY)) {
			_tmp0 = (TVar**)_tmp->Value;
			_tmp = _tmp0[index];
		}
		else {
			printf("Index Out of Bounds\n");
		}
	}
	var = _tmp;
	if (index != -1 && _tmp0 && assignVar) {
		_tmp0[index] = assignVar;
	}

	Result->Value = var->CloneTVar((char*)"var");
	Result->stk = ArrayRes.stk;
}

void TGrammar::CalcOneStep_process_sign_array(TStack* stk, TExpressionResult* Result)
{
	// process braces of array [ ... ]
	int array_len = 0;
	stk = stk->next;
	while (stk->data[0] != '[') {
		*Result = CalcOneStep(stk);
		push(&VarStack, Result->Value);
		//printf("%.2f ", *(double*)(TVar*)(res.Value)->Value);
		array_len++;
		stk = Result->stk;
	}
	stk = stk->next; // pass '['
	TVar* var_array = new TVar((char*)"array", NULL, 0);
	TVar** array = new TVar * [array_len];
	var_array->VarType = EVAR_TYPE_ARRAY;
	var_array->TensorSize = array_len;
	var_array->Value = (void*)array;
	for (int i = 0; i < array_len; i++) {
		array[i] = pop(&VarStack);
	}
	Result->Value = var_array;
	Result->stk = stk;
}

TExpressionResult TGrammar::CalcOneStep(TStack* stk)
{
	TExpressionResult res = { 0, NULL, false }, res_a, res_b;
	if (!stk) {
		printf("Expression execution failed\n");
		return res;
	}
	char znak = stk->data[0];
	if (GetSignPriority(znak) == NOT_A_SIGN) { // it is not a sign, but a Name (var, func, array or const)
		TVar* var;
		if (ReadName(stk->data, 0) != BAD_NAME) { // is it var, func or array?
			if ((var = TableVars->Search(stk->data)) != NULL) { // if it is name of var, so it should be already known (except 'var = ...' )
				if (var->VarType == EVAR_TYPE_DOUBLE)
				{
					CalcOneStep_process_var_double(var, stk, &res);
					return res;
				}
				if (var->VarType == EVAR_TYPE_ARRAY)
				{
					CalcOneStep_process_var_array(var, stk, &res, NULL);
					return res;
				}
				if (var->VarType == EVAR_TYPE_FUNC)
				{
					CalcOneStep_process_var_func(var, stk, &res);
					return res;
				}
			}
			printf("Var '%s' was not found\n", stk->data);
			res.Value = NULL;
		}
		else if (ReadFloat(stk->data, 0) != BAD_FLOAT) // it is a const?			
		{
			CalcOneStep_process_var_const(stk, &res);
			return res;
		}
	}
	else { //znak
		res_a = CalcOneStep(stk->next); // calc first argument of binary operator
		if (!res_a.Value) {
			return res_a;
		}
		if (znak == '=') {
			TVar* _arr = TableVars->Search(res_a.stk->data);
			if (_arr && _arr->VarType == EVAR_TYPE_ARRAY /* && (znak == ']')*/)
			{	// we in situation when element of array is assigned: m[1,2] = 3;
				stk = stk->next; // pass '='
				stk = stk->next; // pass first argument: '3'
				CalcOneStep_process_var_array(_arr, stk, &res_b, res_a.Value);
			}
			else { // just assign y=5 or n=arr[2,1]
				TableVars->Add(res_a.stk->data, res_a.Value);
				res.Value = res_a.Value->CloneTVar((char*)"apply=");
				res.stk = res_a.stk;
			}

			return res;
		}
		if (znak == ']') {
			CalcOneStep_process_sign_array(stk, &res);
			return res;
		}
		res_b = CalcOneStep(res_a.stk); // calc second argument of binary operator
		if (!res_b.Value) {
			return res_b;
		}

		res.Value = ApplySign(res_a.Value, znak, res_b.Value); // apply binary operator
		res.stk = res_b.stk;
		return res;
	}
	return res;
}
