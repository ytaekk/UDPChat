#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char* message);
int main(int argc, char* argv[]) {

	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];
	char tmp[100];
	int strLen;
	int szservAddr;


	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	printf("Program Name: %s\n", argv[0]);
	printf("IP Address: %s\n", argv[1]);
	printf("Port: %s\n", argv[2]);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr);
	servAddr.sin_port = htons(atoi(argv[2]));


	//if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	//	ErrorHandling("connect() error!");
	
	szservAddr = sizeof(servAddr);

	while (1) {
		printf("�޽��� �Է� >>");
		gets_s(tmp, sizeof(tmp));
		//	gets_s(tmp)�� �ϸ� might not be zero-termianted ��� �߻�.
		//	gets_s�� �̹� ���� �����÷ο츦 �����ϰ� �׼� NULL���� ���ڷ� ���� ��Ű�� ���ε��� �� ��� ��Ÿ���ٸ�
		//	gets_s�Լ��� tmp �迭�� ũ�Ⱑ ����� ū�� �������� ���ϴ� ��ó�� ���̱� ����.
		//	�̹� main���� tmp[100]���� ����ϰ� �����ߴµ� gets_s���� ���޴� ������
		//  �Լ��� �迭�� ���ڷ� �����ϸ�, �Լ��� �� �迭�� ���� �ּҸ��� �ް� �˴ϴ�. 
		//	��, �迭 ��ü�� �Լ��� ���޵Ǵ� ���� �ƴ϶�, �迭�� ù ��° ��Ҹ� ����Ű�� �����Ͱ� ���޵˴ϴ�.
		//	�̷� ����, �Լ� ���ο����� ���޹��� �迭�� ���� ũ�⸦ �� �� ���� �˴ϴ�. �׷��� gets_s���� ũ�⸦ ����������
		sendto(hSocket, tmp, strlen(tmp) + 1, 0, (SOCKADDR*)&servAddr, sizeof(servAddr));

		strLen = recvfrom(hSocket, message, sizeof(message) - 1, 0, (SOCKADDR*)&servAddr, &szservAddr);
		//	���⼭ ������ ���ڴ� �������� �ϴ� ������ ����� ���� ���ε� sendto������ sizeof(servAddr)�� ����������,
		//	recvfrom������ sizeof�� �ϸ� unsigned longlong �ڷ����� �Ǿ� ������ ����.
		//	�� ������ sendto�� ������ ���ڴ� �Ϲ����� int�̰� recvform�� int * �������̱� �����̴�.

		/*  [GPT �亯 ����]
		
		*/


		if (strLen <= 0)
			ErrorHandling("read() error!");
		printf("Message from server : %s \n", message);
	}

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}