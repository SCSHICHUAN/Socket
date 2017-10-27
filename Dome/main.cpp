

#pragma warning(disable:4996)//��Ҫ��4996����
#pragma comment(lib,"ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>



void main(void)
{
	WSADATA		wsaData; //��ʼ��socket�õ�
	SOCKET		ListeningSocket; //��������socket�׽���
	SOCKET		NewConnection;  //�ͷ��˵��׽���
	SOCKADDR_IN	ServerAddr;     //����˵�ַ
	SOCKADDR_IN	ClientAddr;    //�ͷ��˵�ַ
	int			ClientAddrLen;  //�ͻ��˵�ַ�ĳ���
	int			Port = 5150;  //�������Ķ˿ڣ�һ��Խ��Խ�ò�Ҫ�ظ����˿�һ����������
	int			Ret;           //
	char		DataBuffer[1024];//����ͷ��˷��͵�����
	//1.0��ʼ��windows�׽��������:��WSAStartup��,(2,2)��2.2�౾������windows�����µİ汾��2.2
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStartup failed with error %d\n", Ret);
		return;
	}
	//2.����socket����socket������AF_INET�� ��intermetЭ�顣��SOCK_STREAM�� ����ʽ�׽��� ����IPPROTO_TCP����TCPЭ��             
	if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket failed with error %d\n", WSAGetLastError());
		WSACleanup();
		return;
	}

	ServerAddr.sin_family = AF_INET;//��ַ��Э�飬�õ��� intermetЭ�顣��sin_family����˼��Э�顣��AF_INET����˼intermet
	ServerAddr.sin_port = htons(Port);//ָ���˿ڣ���htons���������ֽ�ת��δ�����ֽ�
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ָ����ַ���κ�һ����ַ���������ӽ�����

	//3.���׽��ֺ͵�ַ�󶨣���bind��,�����ַ���ж˿ں͵�ַ
	if (bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("bind failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	//4.���÷���˵��׽��ּ�����listen��, ����������
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
		printf("listen failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}

	printf("We are waiting a connection on port %d.\n", Port);
	printf("Listen(���ڼ���)...\n");

	ClientAddrLen = sizeof(SOCKADDR);


	//5.���տͷ��ˡ�accept��,�õ��ͷ��˵��׽���NewConnection
	if ((NewConnection = accept(ListeningSocket, (SOCKADDR*)&ClientAddr, &ClientAddrLen)) == INVALID_SOCKET)
	{
		printf("accept failed with error %d\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return;
	}
	                                                       //����ת��Ϊ����                 
	printf("We successfully got a connection from %s:%d\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

	closesocket(ListeningSocket);
	printf("We are waiting to receive data\n");
	
	
	//6.�ÿͷ��˵��׽���ȥ����,�ͷ��˵�����,������recv��
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