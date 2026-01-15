#include "core.h"
TContext Context;
DWORD WINAPI Console_Thread(void* _core)
{
	char data[ConsoleCommandSize + 1];
	DWORD numRead;
	TCore* Core=(TCore*)_core;
	HANDLE hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ScienceStudio"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	if (hPipe != INVALID_HANDLE_VALUE) {
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)   // wait for someone to connect to the pipe
		{
			while (ReadFile(hPipe, data, ConsoleCommandSize, &numRead, NULL) != FALSE)
			{	
				if (numRead > 0) {
					Core->ConsoleOutput(data, numRead);
				}
			}
		}
	}
	return 0;
}
void TCore::Prompt()
{
	printf("\n>");
}
void TCore::Execute() {
	if ((strcmp(ConsoleInput, "exit") == 0) ||
		(strcmp(ConsoleInput, "Exit") == 0))
	{
		exit(0);
	}
}

void TCore::ConsoleOutput(char* _buffer, uint32_t _size)
{
	FILE* f;
	errno_t err;
	if (Context->FileName) {
		err = fopen_s(&f, Context->FileName, "ab");
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
	Prompt();
}
