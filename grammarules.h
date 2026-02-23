#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
const char SPEC_SYMBOL = '\\';
const char DELIMITER_SYMBOL = '|';
const int RULE_LEN = 100;
const int RULES_TOTAL = 5;
const int MAX_STACK_SIZE = 10000;

static const char Rules[RULES_TOTAL][RULE_LEN] = {
	"S->[A][B]d[C]",
	"A->a\\[|a[A]",
	"B->b|b[B]",
	"C->c|c[C]",
	""
};
struct TQueue {
	char name[RULE_LEN];
	int rule_num;	
};

class GrammaRules {
	struct TQueue** Stack;
public:
	int stk_len, stk_curr;
public:
	int CheckCorrectness(char* input, int _offset, int _rule_row, int _rule_col);
	bool NextCheckPath(int LastQueuePosition);

	void Show();

	char* GetRule(int i, int j);
	int GetRuleCount(int i);
	int Search(char* _rule);
	bool isNameChar(unsigned char ch);
	bool isDigit(char ch);
	char* ReadName(char* _rule, int _offset);
	const char* GetRule(int i) {
		return Rules[i];
	};
	GrammaRules() {
		Stack = new TQueue * [MAX_STACK_SIZE];
		stk_len = 0;
		stk_curr = 0; // взятый элемент из очереди
	}
	~GrammaRules() {
		delete [] Stack; 
		stk_len = 0;
		stk_curr = 0;
	}
	TQueue* pop();
	void push(char* name, int rule_num);
	bool CheckGrammarInput(char* input);
};