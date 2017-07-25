#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#define MAX_BUFFER 1024
#define SERVER_PORT 4977

using namespace std;

class MySocket {
private:
	SOCKET listenSocket;
	SOCKADDR_IN clientAddr;
	SOCKET clientSocket;
	int addrLen;

public:
	MySocket() {
	}

	bool mInit() {
		WSADATA WSAData;
		if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		{
			printf("Error - WSAStartup");
			return 0;
		}

		listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listenSocket == INVALID_SOCKET)
		{
			printf("Error - Invalid socket\n");
			return 0;
		}

		SOCKADDR_IN serverAddr;
		memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
		serverAddr.sin_family = PF_INET;
		serverAddr.sin_port = htons(SERVER_PORT);
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		{
			printf("Error - Fail ttto bind\n");
			closesocket(listenSocket);
			WSACleanup();
			return 0;
		}

		if (listen(listenSocket, 5) == SOCKET_ERROR)
		{
			printf("Error - Fail to listen\n");
			closesocket(listenSocket);
			WSACleanup();
			return 0;
		}

		addrLen = sizeof(SOCKADDR_IN);
		memset(&clientAddr, 0, addrLen);

		return 1;
	}

	bool mAcceptClient() {
		bool boolResult = true;
		clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &addrLen);
		cout << "클라이언트 accept" << endl;
		if (clientSocket == INVALID_SOCKET)
			boolResult = false;
		
		return boolResult;
	}

	int mReceiveMessage(char* buff) {
		int numOfBytes = recv(clientSocket, buff, MAX_BUFFER, 0);
		cout << "get : " << buff << endl;
		int length = 0;

		while (buff[length] != '\r' && buff[length] != '\n')
			length++;

		buff[length] = '\0';
		return numOfBytes;
	}

	void mSendMessage(char* buff) {
		send(clientSocket, buff, strlen(buff), 0);
	}

	void mCloseSocket() {
		closesocket(clientSocket);
	}
};