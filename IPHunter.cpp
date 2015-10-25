#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <math.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")


int main()
{
	//��������������
	unsigned int count=0;

	 //��ʼ��WSA
     WORD sockVersion = MAKEWORD(2,2);
     WSADATA wsaData;
     if(WSAStartup(sockVersion, &wsaData)!=0)
     {
         return 0;
     }
 
     //�����׽���
     SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if(slisten == INVALID_SOCKET)
     {
         printf("socket error !");
         return 0;
     }
 
     //��IP�Ͷ˿�
	 u_short portNum=80;
     sockaddr_in sin;
     sin.sin_family = AF_INET;
     sin.sin_port = htons(portNum);
     sin.sin_addr.S_un.S_addr = INADDR_ANY; 
     if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
     {
TAG1:   printf("bind error ! port %d is used !\n",portNum);
		//reset port
		printf("please input new port:\n");
		scanf("%us",&portNum);
		sin.sin_port = htons(portNum);
		while(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			goto TAG1;
		}
     }
 
     //��ʼ����
     if(listen(slisten, 5) == SOCKET_ERROR)
     {
         printf("listen error !");
		 system("pause");
         return 0;
     }
 
     //ѭ����������
     SOCKET sClient;
     sockaddr_in remoteAddr;
     int nAddrlen = sizeof(remoteAddr);
     char revData[255]; 
     while (true)
     {

		 printf("\n%u ___________________________________________________________\n",count);
         printf("waiting Linking...\n");
         sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
         if(sClient == INVALID_SOCKET)
         {
             printf("accept error !");
             continue;
         }
         printf("accept a link��%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		 if (count<UINT_MAX)
		 {
			 count++;
		 }

		 struct tm *local; 
		 time_t t; 
		 t=time(NULL); 
		 local=localtime(&t); 
		 printf("____Local hour is: %d/%d/%d-%d:%d:%d_____\n",local->tm_year+1900,(local->tm_mon+1),local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec); 

         //��������
         int ret = recv(sClient, revData, 255, 0);        
         if(ret > 0)
         {
             revData[ret] = 0x00;
             printf(revData);
         }
		 //closesocket(sClient);
 
         //��������
		 char * sendData = "<html><head> <title>����ʧ��</title><head/><html/>";
		 send(sClient, sendData, strlen(sendData), 0);
		 closesocket(sClient);
     }
     
     closesocket(slisten);
     WSACleanup();
     return 0;

	
}