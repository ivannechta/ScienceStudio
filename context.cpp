#include "context.h"
#include "header.h"

void TContext::InitArgs()
{
	struct TArguments* ARGS = (struct TArguments*)Arguments;
	ARGS->argc = 0;
	ARGS->argv = new char* [MAX_PARAMS];
	for (uint16_t i = 0; i < MAX_PARAMS; i++) {
		ARGS->argv[i] = NULL;
	}
}

void TContext::AddArgs(char* _param)
{
	struct TArguments* ARGS = (struct TArguments*)Arguments;
	char* arg;

	if (ARGS->argc == MAX_PARAMS - 1) return;

	arg = new char[257];
	strncpy_s(arg, 256, _param, 256);
	ARGS->argv[ARGS->argc] = arg;
	ARGS->argc++;
}

void TContext::ClearArgs()
{
	struct TArguments* ARGS = (struct TArguments*)Arguments;
	for (uint16_t i = 0; i < MAX_PARAMS; i++) {
		if (ARGS->argv[i] != NULL) {
			delete ARGS->argv[i];
			ARGS->argv[i] = NULL;
		}
	}
}
