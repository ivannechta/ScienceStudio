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
    struct TStack* New=new TStack;
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

int TGrammar::Priority(char operation)
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
    int i=_pos;
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
void TGrammar::PolizArithm(char* _expression)
{
	TVar* var = NULL;
	int i = 0;
    int j = 0;
    int size = (int)strlen(_expression);
	char* stk_znak = NULL;
    char* str; // part of the expression with name or float
    bool StartLexemma = true; // unary minus possible, but after name just arithmetic minus appears
    while (i < size) {
        /*printf("Stack \n");
        ShowStack(Stack);
        printf("\n");
        ShowStack(Stack_tmp);
        printf("\n-------\n");*/
        if (((j = ReadName(_expression, i)) != -1) ||   // is it Name or const?
            (StartLexemma && (j = ReadFloat(_expression, i)) != -1)||
            (!StartLexemma && (j = UReadFloat(_expression, i)) != -1))
        {
            StartLexemma = false;
            str = GetNewStr(_expression, i, j);
			var = TableVars->Search(str);
            //if (strcmp(str, "func") == 0) { //Fix checking function by VAR/func search
            if (var && 
                (var->VarType == EVAR_TYPE_FUNC ||
                 var->VarType == EVAR_TYPE_ARRAY)){
                push(&Stack_tmp, str);
            }
            else {
                push(&Stack, str);
            }
			i = j + 1;
            continue;
        }
        //--------------- ZNAK----------------------
        char* znak = new char[2];
        znak[0] = _expression[i];
        znak[1] = 0;
        if (Priority(znak[0]) != 0) // it is a operation symbol
        {   
            StartLexemma = false;
            if (Stack_tmp == NULL) {
                
                push(&Stack_tmp, znak);
            } else {
                if ((znak[0] == '(') ||
                    (znak[0] == '['))

                {
                    push(&Stack_tmp, znak);
                    if (znak[0] == '['){ push(&Stack, znak); }
                    i++;
                    continue;
                }

                if ((znak[0] == ')') || (znak[0] == ']') ||
                    (znak[0] == ',')) // function delimiter
                {
                    while ((Stack_tmp) &&
                        (Stack_tmp->data[0]!='(') &&
                        (Stack_tmp->data[0] != '['))
                    {
                        stk_znak = pop(&Stack_tmp);
                        if (stk_znak) {
                            push(&Stack, stk_znak);
                        }
                    }
                    if (znak[0] != ',') {
                        pop(&Stack_tmp); // pop '('
                        if (znak[0] == ']') { push(&Stack, znak); }
                    }
                    var = TableVars->Search(Stack_tmp->data);
                    if (var && 
                            ((var->VarType == EVAR_TYPE_FUNC)||
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
                } else {
                    while ((Stack_tmp) &&
                        (Priority(znak[0]) <= Priority(Stack_tmp->data[0])) &&
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
	
	while (Stack_tmp) {
		stk_znak = pop(&Stack_tmp);
		if (stk_znak) {
			push(&Stack, stk_znak);
		}
	}
}
void TGrammar::CalcExpr(char* _expression) {
    PolizArithm(_expression);
    TExpressionResult res = CalcOneStep(Stack);
    if (res.Value) {
        TableVars->ShowVar(res.Value);
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
double TGrammar::ApplySign(double _a,char znak ,double _b) { 
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
        if ((_a->VarType == EVAR_TYPE_FLOAT) &&
            (_b->VarType == EVAR_TYPE_FLOAT)) {
            *d = *(double*)_a->Value + *(double*)_b->Value;
            var = new TVar((char*)"ApplySign", d, sizeof(double));            
            var->TensorSize = 1;
        }
        else { return NULL; }
        return var;
    case '-': 
        if ((_a->VarType == EVAR_TYPE_FLOAT) &&
            (_b->VarType == EVAR_TYPE_FLOAT)) {
            *d = *(double*)_a->Value - *(double*)_b->Value;
            var = new TVar((char*)"ApplySign", d, sizeof(double));            
            var->TensorSize = 1;
        }
        else { return NULL; }
        return var;        
    case '*': 
        if ((_a->VarType == EVAR_TYPE_FLOAT) &&
            (_b->VarType == EVAR_TYPE_FLOAT)) {
            *d = *(double*)_a->Value * *(double*)_b->Value;
            var = new TVar((char*)"ApplySign", d, sizeof(double));            
            var->TensorSize = 1;
        }
        else { return NULL; }
        return var;
    case '/': 
        if ((_a->VarType == EVAR_TYPE_FLOAT) &&
            (_b->VarType == EVAR_TYPE_FLOAT)) {
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

TExpressionResult TGrammar::CalcOneStep(TStack* stk )
{
    ModuleFuncType FunctionCaller;
    
	TExpressionResult res = { 0,NULL }, res_a, res_b;
    if (!stk) { 
        printf("Expression execution failed\n");
        return res;
    }
    char znak = stk->data[0];
    if (Priority(znak) != 0) { //znak
        res_a = CalcOneStep(stk->next);
        if (!res_a.Value) {
            return res_a;
        }
        if (znak == '=') {
            //TableVars->ShowVar(res_a.Value);
            TableVars->Add(res_a.stk->data, res_a.Value);
            res.Value = res_a.Value->CloneTVar((char*)"apply=");
            res.stk = res_a.stk;
            return res;
        }
        if (znak == ']') {
            int array_len = 0;
            stk = stk->next;
            while(stk->data[0]!='[') {
                res = CalcOneStep(stk);
                push(&VarStack, res.Value);
                //printf("%.2f ", *(double*)(TVar*)(res.Value)->Value);
                array_len++;
				stk = res.stk;
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
            res.Value = var_array;
            res.stk = stk;
            return res;
        }
        res_b = CalcOneStep(res_a.stk);
        if (!res_b.Value) {
            return res_b;
        }
        
        res.Value = ApplySign(res_a.Value, znak, res_b.Value);
        res.stk = res_b.stk;
        return res;
    } else { // it is not a sign, but a Name (var or func)
        TVar* var; double* var_tmp;
		if (ReadName(stk->data, 0) != -1) {
			if ((var = TableVars->Search(stk->data)) != NULL) { // if it is name of var, so it should be already known (except 'var = ...' )
                if (var->VarType == EVAR_TYPE_FLOAT) {
                    res.Value = var->CloneTVar((char*)"var");
                    res.stk = stk->next;
                    return res;
                }
                else {
                    // TODO
                    // create args array
                    struct TArguments* Args = (TArguments*)Context->Arguments;
                    TVar* FuncArg;
                    TExpressionResult FuncRes;
                    Args->argc = var->TensorSize; // argument counts
                    Args->argv = new TVar*[Args->argc];
                    FuncRes.stk = stk->next;
                    for (int i = 0; i < Args->argc; i++) {
                        FuncRes = CalcOneStep(FuncRes.stk); // Calc every argument
                        if (!FuncRes.Value) {
                            return FuncRes;
                        }
                        FuncArg = FuncRes.Value->CloneTVar((char*)"funcArg");//new TVar((char*)"funcArg", &FuncRes.Value, sizeof(double));						
                        FuncArg->TensorSize = 1;
                        Args->argv[i] = FuncArg; // put calculated argument in structure for transmitt to dll
                    }
                    // call func & return result
                    FunctionCaller = (ModuleFuncType)(*(ModuleFuncType*)(var->Value));
                    TVar* FuncResult = FunctionCaller();
                    // TODO check what type was returned: double or string or array Name of Tvar contains a type
					res.Value = FuncResult;
                    res.stk = FuncRes.stk;
                    // delete arguments form memory after func call(free)
                    for (int i = 0; i < Args->argc; i++) {
                        delete Args->argv[i];
                    }
                    //delete FuncResult;
                    return res;
                }
            }
            printf("Var not found\n");
            res.Value = NULL;
		}
		else if ((ReadFloat(stk->data, 0) != -1) ||
                (ReadFloat(stk->data, 0) != -1))
        {
            TVar* var;
            double* _d = new double;
            *_d = atof(stk->data);
            var = new TVar((char*)"Num", _d, sizeof(double));            
            var->TensorSize = 1;
            var->Other = NULL;
            res.Value = var;
            res.stk = stk->next;
            return res;
		}
    }
    return res;
}
