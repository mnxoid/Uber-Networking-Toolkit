#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

void Getch()
{
	while (getchar() != '\n');
}

int main(int argc, char ** argv)
{
	string ipStr;
	int port;
	if (argc > 1)
	{
		ipStr = string(argv[1]);
		port = atoi(argv[2]);
	}
	else 
	{
		cout << "Usage:\n-------------------------\n     tcp2std ip port\n-------------------------\n";
		cout << "Input server address: ";
		cin >> ipStr;
		Getch();
		cout << "Input port number: ";
		cin >> port;
		Getch();
	}
	cout << endl;
	cout << "######################################" << endl;
	cout << "#                                    #" << endl;
	cout << "# UBER CLIENT by Mnxoid x Icebreaker #" << endl;
	cout << "#                                    #" << endl;
	cout << "######################################" << endl;
	cout << "Connecting to " << ipStr << ":" << port << "..." << endl;
	Getch();
	return 0;
}