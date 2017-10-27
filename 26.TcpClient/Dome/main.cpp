

#pragma warning(disable:4996)//��Ҫ��4996����
#pragma comment(lib,"ws2_32.lib")//lib��̬���ӿ�

#include<WinSock2.h>//socket2.2�İ汾
#include<stdio.h>
#include<stdlib.h>

//        �����в���  
void main(int argc,char **argv)
{

	WSADATA wsaData;
	SOCKET  s;
	SOCKADDR_IN ServerAddr;
	int     Ret;
	int     Port = 5150;
	
	//���������в���
	if (argc <= 1)
	{
		printf("USAGE: TCPClinet <Server IP address>.\n");
		return;
		
	}


	//1.���غͳ�ʼ�������
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		printf("WSAStarup faiked with error. %d\n", Ret);
		return;
	}

	//2.�����׽���
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("socket filed with error. %d\n", WSAGetLastError());
		WSACleanup();
	}

	//3.��ʼ����
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port); //���ӵĶ˿�
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); // argv[1] �����в���    �������п�������IP��ַ      /*inet_addr("127.0.0.1"); �����ĵ�ַ*/

	printf("We are trying to  connect to %s:%d...\n", inet_ntoa(ServerAddr.sin_addr),htons(ServerAddr.sin_port));

	//4.����
	if ((connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr))) == SOCKET_ERROR)
	{
		printf("connect failed with error. %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}

	printf("Our connect susseeded\n");
	printf("We will now try to send a \"hello\" to  servers.\n ");

	char *cs = "hello socket";
	long size = sizeof(cs);

	printf("%d\n", size);

	//5.����
	if ((Ret = send(s, cs, 12, 0)) == SOCKET_ERROR)
	{
		printf("Sent failed with error %d.\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}
	printf("We successfilly sent %d batys.\n", Ret);

	closesocket(s);
	WSACleanup();


	system("pause");
}
