#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

void main(void)
{
	WSADATA		wsaData;
	SOCKET		ListeningSocket;
	SOCKET		NewConnection;
	SOCKADDR_IN	ServerAddr;
	SOCKADDR_IN	ClientAddr;
	int			ClientAddrLen;
	int			Port = 5150;
	int			Ret;
	char		DataBuffer[1024];

	if((Ret = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", Ret);
		return;
	}
	
	if((ListeningSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if(bind(ListeningSocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("bind failed with error %d\n",WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}

	if(listen(ListeningSocket,5) == SOCKET_ERROR)
	{
		printf("listen failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}

	printf("We are waiting a connection on port %d.\n",Port);
	printf("Listen(ÕýÔÚ¼àÌý)...\n");

	ClientAddrLen = sizeof(SOCKADDR);
	if((NewConnection = accept(ListeningSocket,(SOCKADDR*)&ClientAddr,&ClientAddrLen)) == INVALID_SOCKET)
	{
		printf("accept failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}

	printf("We successfully got a connection from %s:%d\n", inet_ntoa(ClientAddr.sin_addr),ntohs(ClientAddr.sin_port));

	closesocket(ListeningSocket);
	printf("We are waiting to receive data\n");

	if((Ret = recv(NewConnection,DataBuffer, sizeof(DataBuffer),0)) == SOCKET_ERROR)
	{
		printf("recv failed with error %d\n", WSAGetLastError());
		closesocket(NewConnection);
		WSACleanup();
		return;
	}

	printf("We successfully received %d bytes.\n", Ret);
	DataBuffer[Ret] = '\0';
	printf("%s\n", DataBuffer);
	printf("We are now going to close the client connection.\n");
	closesocket(NewConnection);
	WSACleanup();
	system("pause");
}