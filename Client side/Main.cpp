#define _UNICODE
#define UNICODE

#include "stdafx.h"
#include "targetver.h"

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>


bool loggedIn = false;

using namespace std;

struct UserCredentials
{
	char name[16];
	char lastName[16];
	char Username[16];
	char password[16];
};

int main(int argc, char* argv[])
{
	SOCKET clientSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		cout << "the winsock dll not found " << endl;
		return 0;
	}
	else {
		cout << "the winsock dll found " << endl;
		cout << "the status  " << wsaData.szSystemStatus << endl;

	}

	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		cout << "error at socket()" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "socket is ok " << endl;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPtonA(AF_INET, ("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		cout << "client connect() - failed to connect." << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "client : connect() is ok." << endl;
		cout << "client: can start sending and recieving data..." << endl;
	}
	bool exitSocket = false;
	//talking to server
	char buffer[200];

	char exit[200] = "SHUTDOWN";

	char loginPrompt[16];

	if(!loggedIn)
	{
		cout << "enter your name (max of 16 characters) " << endl;
		cin.getline(buffer, 16);
		
		int byteCount = send(clientSocket, buffer, 16, 0);

		if (byteCount > 0)
		{
			cout << "You said name " << buffer << endl;
			
		}
		else {
			WSACleanup();
		}
		cout << "enter your last name(max of 16 characters) " << endl;
		cin.getline(buffer, 16);

		byteCount = send(clientSocket, buffer, 16, 0);
		if (byteCount > 0)
		{
			cout << "you said your last name was: " << buffer << endl;
		}
		else {
			WSACleanup();
		}
		loggedIn = true;
	}
	
	while (!exitSocket && loggedIn)
	{
		cout << "enter your message to server" << endl;
		cin.getline(buffer, 200);

		int byteCount = send(clientSocket, buffer, 200, 0);

		if (strcmp(buffer, exit) == 0)
		{
			cout << "you entered the exit code shutting down" << endl;
			exitSocket = true;
			WSACleanup();
			return 0;
		}

		if (byteCount > 0)
		{
			cout << "You Said: " << buffer << endl;

		}
		else {
			WSACleanup();
		}
		byteCount = recv(clientSocket, buffer, 200, 0);
		if (byteCount > 0)
		{
			cout << "Server Said: " << buffer << endl;
		}
		else {
			WSACleanup();
		}
	}

	system("pause");
	WSACleanup();




	return 0;
}
