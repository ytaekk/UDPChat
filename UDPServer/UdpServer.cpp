#include <winsock2.h> // ��������� ������ ��� �� 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <istream>

#pragma comment(lib, "ws2_32.lib")


void ErrorHandling(const char* message);

int main(int argc, char* argv[]) {

	WSADATA wsaData; // ������������ȭ 
	SOCKET hServSock;// hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[30];
	int strLen;


	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ���������̱� ������ WSAStartup �� ��.
	//���� ����� ���� ���µ�, MAKEWORD �� ���� Ȯ�ο��̶�� �����ϸ� �ȴ�. 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);	// UDP : SOCK_DGRAM / TCP,IP: SOCK_STREAM
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	// Listen() �Լ��� UDP���� �ʿ����.

	/*if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");*/	

	
	//hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	//if (hClntSock == INVALID_SOCKET)
	//	ErrorHandling("accept() error");

	while (1) {
		szClntAddr = sizeof(clntAddr);
		memset(message, '\0', 30);
		// UDP : recvfrom() , TCP : recv()
		strLen = recvfrom(hServSock, message, sizeof(message) - 1, 0, (SOCKADDR*)&clntAddr,&szClntAddr);
		if (strLen == -1)
			ErrorHandling("recvfrom() error!");
		printf("Message from client : %s \n", message);

		printf("�޽��� �Է� >>");
		gets_s(message);
		//	UPD:sendto , TCP: send
		sendto(hServSock, message, strlen(message) + 1, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));

	}

	// closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}