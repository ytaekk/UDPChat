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
		printf("메시지 입력 >>");
		gets_s(tmp, sizeof(tmp));
		//	gets_s(tmp)만 하면 might not be zero-termianted 경고 발생.
		//	gets_s는 이미 버퍼 오버플로우를 방지하고 항성 NULL종료 문자로 종료 시키는 것인데도 이 경고가 나타난다면
		//	gets_s함수가 tmp 배열의 크기가 충분히 큰지 인지하지 못하는 것처럼 보이기 떄문.
		//	이미 main에서 tmp[100]으로 충분하게 선언했는데 gets_s에서 못받는 이유는
		//  함수에 배열을 인자로 전달하면, 함수는 그 배열의 시작 주소만을 받게 됩니다. 
		//	즉, 배열 자체가 함수로 전달되는 것이 아니라, 배열의 첫 번째 요소를 가리키는 포인터가 전달됩니다.
		//	이로 인해, 함수 내부에서는 전달받은 배열의 실제 크기를 알 수 없게 됩니다. 그래서 gets_s에서 크기를 명시해줘야함
		sendto(hSocket, tmp, strlen(tmp) + 1, 0, (SOCKADDR*)&servAddr, sizeof(servAddr));

		strLen = recvfrom(hSocket, message, sizeof(message) - 1, 0, (SOCKADDR*)&servAddr, &szservAddr);
		//	여기서 마지막 인자는 보내고자 하는 소켓의 사이즈를 적는 것인데 sendto에서는 sizeof(servAddr)로 가능하지만,
		//	recvfrom에서는 sizeof로 하면 unsigned longlong 자료형이 되어 오류가 난다.
		//	그 이유는 sendto의 마지막 인자는 일반적인 int이고 recvform은 int * 포인터이기 때문이다.

		/*  [GPT 답변 보기]
		
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