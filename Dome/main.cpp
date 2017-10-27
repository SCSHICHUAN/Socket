

#pragma warning(disable:4996)//不要有4996错误
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>



void main(void)
{
	WSADATA		wsaData; //初始化socket用的
	SOCKET		ListeningSocket; //服务器的socket套接字
	SOCKET		NewConnection;  //客服端的套接字
	SOCKADDR_IN	ServerAddr;     //服务端地址
	SOCKADDR_IN	ClientAddr;    //客服端地址
	int			ClientAddrLen;  //客户端地址的长度
	int			Port = 5150;  //服务器的端口，一般越大越好不要重复。端口一共有六万多个
	int			Ret;           //
	char		DataBuffer[1024];//缓存客服端发送的数据
	//1.0初始化windows套接字网络库:“WSAStartup”,(2,2)：2.2坂本。现在windows的最新的版本是2.2
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", Ret);
		return;
	}
	//2.创建socket：“socket”。“AF_INET” ：intermet协议。“SOCK_STREAM” ：流式套接字 。“IPPROTO_TCP”：TCP协议             
	if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	ServerAddr.sin_family = AF_INET;//地址的协议，用的是 intermet协议。“sin_family”意思是协议。“AF_INET”意思intermet
	ServerAddr.sin_port = htons(Port);//指定端口，“htons”：本机字节转换未网络字节
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//指定地址，任何一个地址都可以链接进来，

	//3.把套接字和地址绑定，“bind”,这个地址里有端口和地址
	if (bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("bind failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	//4.利用服务端的套接字监听“listen”, 这里监听五个
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
		printf("listen failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}

	printf("We are waiting a connection on port %d.\n", Port);
	printf("Listen(正在监听)...\n");

	ClientAddrLen = sizeof(SOCKADDR);


	//5.接收客服端“accept”,得到客服端的套接字NewConnection
	if ((NewConnection = accept(ListeningSocket, (SOCKADDR*)&ClientAddr, &ClientAddrLen)) == INVALID_SOCKET)
	{
		printf("accept failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	                                                       //网络转变为本机                 
	printf("We successfully got a connection from %s:%d\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

	closesocket(ListeningSocket);
	printf("We are waiting to receive data\n");
	
	
	//6.用客服端的套接字去接收,客服端的数据,函数“recv”
	if ((Ret = recv(NewConnection, DataBuffer, sizeof(DataBuffer), 0)) == SOCKET_ERROR)
	{
		printf("recv failed with error %d\n", WSAGetLastError());
		closesocket(NewConnection);
		WSACleanup();
		return;
	}

	printf("We successfully received %d bytes.\n", Ret);
	DataBuffer[Ret] = '\0';
	printf("\n");
	printf("----|accept message|----\n");
	printf("\n");
	printf("%s\n", DataBuffer);
	printf("\n");
	printf("----------|end|----------\n");
	printf("\n");
	printf("We are now going to close the client connection.\n");
	closesocket(NewConnection);
	WSACleanup();
	system("pause");
}