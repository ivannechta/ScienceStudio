#include "core.h"
TContext Context;
void TCore::Execute() {
	if ((strcmp(ConsoleInput, "exit") == 0) ||
		(strcmp(ConsoleInput, "Exit") == 0))
	{
		exit(0);
	}
	
	SHOW("Ok\n");
}

void TCore::ConsoleOutput(char* _buffer, uint32_t _size)
{
	FILE* f;
	errno_t err;
	if (Context.FileName) {
		err = fopen_s(&f, Context.FileName, "ab");
		if (err != 0) {
			SHOW("Error. File not found.\n");
			return;
		}
		fwrite(_buffer, 1, _size, f);
		fprintf(f, "\n");
		fclose(f);
	} else {
		_size = (_size > OUTPUT_TEXT_MAX) ? OUTPUT_TEXT_MAX : _size;
		for (uint32_t i = 0; i < _size; i++) {
			printf("%c", _buffer[i]);
		}
		printf("\n");
	}
}
