#include "grammarules.h"

int GrammaRules::CheckCorrectness(char* input, int _offset, int _rule_row, int _rule_col)
{
	char* rule = GetRule(_rule_row, _rule_col);
	char* NT; //NONTerminal	
	TQueue* st;
	int i = 0;
	int tmp, col, row;
	while (i < strlen(rule)) {
		//printf("rule[i] %c\n", rule[i]);
		if (rule[i] == '[') { //find NONTerminal			
			NT = ReadName(rule, i + 1);
			//printf("NT %s\n", NT);
			row = Search(NT);
			st = pop();
			if (st == NULL) { //если не хватило в стеке пути раскрытия, то берем номер 1
				push(NT, 1);
				st = pop();
			}			
			tmp = CheckCorrectness(input, _offset, row, st->rule_num);
			if ((tmp != -1) && (tmp > _offset)) {
				_offset = tmp - 1;
				i += strlen(NT) + 2;//pass name, '[' and ']'
				//printf("expand to rule_i %d input offset %d\n", i, _offset);
				//printf("pass %c\n", rule[i]);				
				_offset++;
			}
			else {
				return -1;
			}
			continue;
		}
		if (rule[i] == SPEC_SYMBOL) {
			i++; // pass '@'
		}
		if (input[_offset] != rule[i]) {
			return -1;
		}
		//printf("pass %c\n", rule[i]);
		i++;
		_offset++;
	}
	return _offset;
}

bool GrammaRules::NextCheckPath(int LastQueuePosition)
{
	if (LastQueuePosition == 0) return false;
	struct TQueue* tmp = Stack[LastQueuePosition - 1];
	int rule_row = Search(tmp->name);
	if (rule_row == -1) {
		printf("Error rule_row in NextCheckPath\n");
		return false;
	}
	//printf("Next %s\n", tmp->name);
	int rule_count = GetRuleCount(rule_row);
	if (rule_count == -1) {
		printf("Error rule_count in NextCheckPath\n");
		return false;
	}
	if (tmp->rule_num < rule_count - 1) {
		tmp->rule_num++;
	}
	else {
		stk_len--;
		return NextCheckPath(LastQueuePosition - 1);
	}
	return true;
}
void GrammaRules::Show() {
	for (int i = 0; i < stk_len; i++) {
		printf("(%s %d) \n", Stack[i]->name, Stack[i]->rule_num);
	}
}

char* GrammaRules::GetRule(int _ruleString, int _RequiredRuleNum)
{
	char* Result = new char[RULE_LEN];
	int curentRule = 1, i = 0;
	int k = 0;
	if (_RequiredRuleNum == 0) {
		while (Rules[_ruleString][i] != '-') { // "->"
			if (Rules[_ruleString][i] == 0) { /*error*/ return NULL; }
			Result[k] = Rules[_ruleString][i];
			k++;
			i++;
		}
		Result[k] = 0;
		return Result;
	}
	//pass "->"
	while (Rules[_ruleString][i] != '-') {
		if ((Rules[_ruleString][i] == 0) || (i == RULE_LEN)) { /*error*/ return NULL; }
		i++;
	}
	i++;
	if (Rules[_ruleString][i] == '>') {
		i++;
	}
	else {
		//error
	}

	while (curentRule != _RequiredRuleNum) {
		if ((Rules[_ruleString][i] == 0) || (i == RULE_LEN)) { /*error*/ return NULL; }
		if (Rules[_ruleString][i] == DELIMITER_SYMBOL) {
			curentRule++;
			i++;
			continue;
		}
		i++;
	}
	// copy result

	while (Rules[_ruleString][i] != DELIMITER_SYMBOL) {
		if (Rules[_ruleString][i] == 0) { break; }
		Result[k] = Rules[_ruleString][i];
		k++;
		i++;
	}
	Result[k] = 0;
	return Result;
}

int GrammaRules::GetRuleCount(int _ruleString)
{
	int Result = 0;
	int curentRule = 1, i = 0;
	int k = 0;
	while (Rules[_ruleString][i] != '-') { // "->"
		if (Rules[_ruleString][i] == 0) { /*error*/ return 0; }
		i++;
	}
	i++;
	if (Rules[_ruleString][i] == '>') {
		i++;
	}
	else {
		//error
	}
	Result++;
	while ((Rules[_ruleString][i] != 0) && (i != RULE_LEN)) {
		if (Rules[_ruleString][i] == DELIMITER_SYMBOL) {
			Result++;
		}
		i++;
	}
	return Result + 1;
}

int GrammaRules::Search(char* _rule)
{
	uint16_t len = strlen(_rule);
	int j;
	for (int i = 0; i < RULES_TOTAL; i++) {
		for (j = 0; j < len; j++) {
			if (_rule[j] != Rules[i][j]) {
				j = len; //break for
				continue;
			}
		}
		if (Rules[i][j] == '-') { // '->'
			return i;
		}
	}
	return -1;
}
bool GrammaRules::isNameChar(unsigned char ch) {
	if (((ch >= 'a') && (ch <= 'z')) ||
		((ch >= 'A') && (ch <= 'Z')) ||
		(ch == '.')) return true;
	return false;
}
bool GrammaRules::isDigit(char ch) {
	return ((ch >= '0') && (ch <= '9'));
}

char* GrammaRules::ReadName(char* _rule, int _offset)
{
	char* Result = new char[RULE_LEN];
	int j = 0;
	int i = _offset;
	if ((_rule[i] == 0) || isDigit(_rule[i])) {
		Result[0] = 0;
		return Result;
	}
	while ((_rule[i] != 0) &&
		(isNameChar(_rule[i])))
	{
		Result[j++] = _rule[i];
		i++;
	}
	if (i == _offset) {
		Result[0] = 0;
		return Result;
	}
	Result[j] = 0;
	return Result;
}

TQueue* GrammaRules::pop()
{
	if (stk_curr < stk_len) {
		return Stack[stk_curr++];
	}
	return NULL;
}

void GrammaRules::push(char* name, int rule_num)
{
	TQueue* tmp = new TQueue;
	strncpy_s(tmp->name, name, strlen(name));
	tmp->rule_num = rule_num;
	Stack[stk_len++] = tmp;
}

bool GrammaRules::CheckGrammarInput(char* input)
{
	bool flag = false;
	int len;
	int strlen_input = strlen(input);

	do {
		stk_curr = 0;
		len = CheckCorrectness(input, 0, 0, 1);
		if (len == strlen_input) {
			return true;
		}
		//Show();
	} while (NextCheckPath(stk_len));
	return false;
}
