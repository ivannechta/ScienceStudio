#include "grammar.h"

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

void TGrammar::push(TStack** _stack, char* _data)
{
    struct TStack* New=new TStack;
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
    //case '(':
    //case '[':
        //return 6;
    //case '^':
//        return 5;
    case '*':
    case '/':
        return 4;
    case '+':
    case '-':
        return 3;
//    case '=':
//        return 2;

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
    }
    return i;
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
        ((ch >= 'A') && (ch <= 'Z'))) return true;
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
char* TGrammar::PolizArithm(char* _expression)
{
	int i = 0;
    int j = 0;
    int size = strlen(_expression);
	char* stk_znak = NULL;
    char* str; // part of the expression with name or float
    bool StartLexemma = true; // unary minus possible, but after name just arithmetic minus appears
    while (i < size) {
        printf("Stack \n");
        ShowStack(Stack);
        printf("\n");
        ShowStack(Stack_tmp);
        printf("\n-------\n");

        if (((j = ReadName(_expression, i)) != -1) ||
            (StartLexemma && (j = ReadFloat(_expression, i)) != -1)||
            (!StartLexemma && (j = UReadFloat(_expression, i)) != -1))
        {
            StartLexemma = false;
            str = GetNewStr(_expression, i, j);
            push(&Stack, str);
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
                while ((Stack_tmp)&&(Priority(znak[0]) <= Priority(Stack_tmp->data[0]))) {
                    stk_znak = pop(&Stack_tmp);
                    push(&Stack, stk_znak);
                }
                push(&Stack_tmp, znak);
            }
            i++;
            continue;
        }
    }
	//    ShowStack(Stack);
	while (Stack_tmp) {
		stk_znak = pop(&Stack_tmp);
		if (stk_znak) {
			push(&Stack, stk_znak);
		}
	}

	ShowStack(Stack);

	return nullptr;
}
