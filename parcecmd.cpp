#include "parcecmd.h"
#include "core.h"
#include "context.h"
bool TParceCmd::isNameChar(unsigned char ch) {
	if (isDigit(ch)) return true;
	if (((ch >= 'a') && (ch <= 'z')) ||
		((ch >= 'A') && (ch <= 'Z'))) return true;
	return false;
}
bool TParceCmd::isDigit(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}
int TParceCmd::PassNameSpace(char* text, int i) {
	int lastPoint = -1;
	int j;
	while (text[i] != '(')
	{
		if (text[i] == '.') {
			lastPoint = i;
			i++;
			continue;
		}
		j = PassName(text, i);
		if (i >= j) return -1; //Cannot read further
		if (j < 0) return -1;

		i = j + 1;
	}
	return lastPoint - 1;
}
int TParceCmd::PassFuncName(char* text, int i)
{
	return 0;
}
int TParceCmd::PassName(char* text, int i)
{	
	if ((text[i] != 0) && isDigit(text[i])) {
		return -1;
	}
	while (text[i] != 0) {
		unsigned char ch = text[i];
		if (!isNameChar(ch)) {
			return i - 1;
		}
		i++;
	}
	return i - 1;
}
bool TParceCmd::ParceFunctionCall(char* text, int i) {
	char* Cons = Core->ConsoleInput;
	int j;
	char* Namespace = new char[256];
	char* Alias = new char[256];
	
	j = PassNameSpace(Cons, i);
	if (j < 0) {
		char _message[] = "Invalid expression in NameSpace\n";
		Core->ConsoleOutput(_message, (uint32_t)strlen(_message));
		return false;
	}
	memcpy(Namespace, &Cons[i], j - i + 1); Namespace[j - i + 1] = 0;
	i = j;
	printf("Recognized namespace %s\n", Namespace);

	j = PassName(Cons, i + 2);
	if (j < 0) {
		char _message[] = "Invalid expression in Function Name\n";
		Core->ConsoleOutput(_message, (uint32_t)strlen(_message));
		return false;
	}
	memcpy(Alias, &Cons[i + 2], j - i - 1); Alias[j - i - 1] = 0;
	i = j;
	printf("Recognized Alias %s\n", Alias);

	TSettings_Record rec;

	rec.AliasName = Alias;
	j = Core->Settings->Search(&rec);
	if (j != -1) {
		ParceParams(Cons, i + 1);
		Core->Settings->ModuleFunction[0]();
	}
	else {
		char _message[] = "Invalid Funcation Name. Not found\n";
		Core->ConsoleOutput(_message, (uint32_t)strlen(_message));
	}
	return true;
}
bool TParceCmd::ParceParams(char* text, int i) //depricated
{
	char* str = &text[i];
	char* context = NULL; // Context pointer for state
	size_t max_len = sizeof(str); // For the first call
	char delim[] = "(, )";

	// First call: tokenize the string by comma
	/*
	token = strtok_s(str, delim, &context); // Note: context and max_len often passed by reference in implementations
	Core->Context_lnk->ClearArgs();
	while (token != NULL) {
		printf("ParcedParams->%s\n", token);
		Core->Context_lnk->AddArgs(token);
		token = strtok_s(NULL, delim, &context);
	}*/
	return true;
}
void TParceCmd::Parce()
{	
	char* Cons = Core->ConsoleInput;
	if (Cons[0] == '.') {
		ParceFunctionCall(Cons, 1);
	}
}