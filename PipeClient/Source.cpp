#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <io.h>
#include <fcntl.h>

#define BUFSIZE 512

HANDLE hPipe;
TCHAR  chBuf[BUFSIZE];
BOOL   fSuccess = FALSE;
DWORD  cbRead, cbToWrite, cbWritten, dwMode;
LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\Sithis");
LPTSTR lpvMessage = TEXT("Default message from client.");
std::string pipeInputName;

void Info() {
	printf("Pipe Client for Uber Networking Toolkit, v1.4.88\n");
	printf("Copyright by Icebreaker, 2015\n");
	printf("In the name of our Dread Lord Sithis and Mother of the Night\n");
	printf("Type \"help\" for help\n");
}

void Help() {
	std::cout
		<< "List of available commands:" << std::endl
		<< "\t send - start sending process" << std::endl
		<< "\t conf - configure pipe name" << std::endl
		<< "\t exit - quit" << std::endl;
}

void Configure() {
	printf("Please, specify a pipe name\n");
	std::getline(std::cin, pipeInputName);
	pipeInputName = "\\\\.\\pipe\\" + pipeInputName;
	TCHAR* pp = new TCHAR[256];
	wcscpy(pp, (const wchar_t*)pipeInputName.c_str());
	lpszPipename = pp;
	printf("%s\n", lpszPipename);
}

void Routine() 
{
	TCHAR* sendMessage = new TCHAR[256];
	std::string confass;
	std::cout
		<< "Message: "
		<< std::endl;
	std::cin >> confass;

	wcscpy(sendMessage, (const wchar_t*)confass.c_str());

	lpvMessage = sendMessage;

	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	getchar();
	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
		{
			break;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			getchar();
			exit(-1);
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			getchar();
			exit(-1);
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		getchar();
		exit(-1);
	}

	// Send a message to the pipe server. 
	cbToWrite = (lstrlen(lpvMessage) + 1)*sizeof(TCHAR);
	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		getchar();
		exit(-1);
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");

	do
	{
		// Read from the pipe. 

		fSuccess = ReadFile(
			hPipe,    // pipe handle 
			chBuf,    // buffer to receive reply 
			BUFSIZE*sizeof(TCHAR),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;

		_tprintf(TEXT("\"%s\"\n"), chBuf);
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if (!fSuccess)
	{
		_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
		getchar();
		exit(-1);
	}

	CloseHandle(hPipe);
}

int _tmain(int argc, TCHAR *argv[])
{
	Info();
	std::string command = " ";
	while (command != "exit")
	{
		std::cout
			<< ">>>";
		std:getline(std::cin, command);
		if (command == "conf") Configure();
		else if (command == "send") Routine();
		else if (command == "help") Help();
		else std::cout
			<< "Command unrecognized"
			<< std::endl;
	}
	// Try to open a named pipe; wait for it, if necessary. 

}