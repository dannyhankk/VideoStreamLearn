// VideoStreamClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <rtmp_sys.h>
#include <stdio.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

typedef RTMP *(*tp_rtmp_alloc)(void);
typedef void (*tp_rtmp_init)(RTMP *);
typedef int (*tp_rtmp_setupurl)(RTMP *, char *);
typedef int (*tp_rtmp_connect)(RTMP *, RTMPPacket *);
typedef int (*tp_rtmp_connectstream)(RTMP *, int);
typedef int(*tp_rtmp_read)(RTMP *, char *, int);
typedef void(*tp_rtmp_close)(RTMP *);

typedef void(*tp_rtmp_setlog_output)(FILE *);

tp_rtmp_alloc rtmpAlloc = NULL;
tp_rtmp_init rtmpInit = NULL;
tp_rtmp_setupurl rtmpSetUpUrl = NULL;
tp_rtmp_connect rtmpConnect = NULL;
tp_rtmp_connectstream rtmpConnectStream = NULL;
tp_rtmp_read rtmpRead = NULL;
tp_rtmp_close rtmpClose = NULL;
tp_rtmp_setlog_output rtmpSetLog = NULL;

bool GetFunction(){
	
	HMODULE handle = LoadLibrary(_T("librtmp.dll"));
	if (handle == NULL)
	{
		int error = GetLastError();
		printf("load library failed, error: %d\n", error);
		return false;
	}
	rtmpAlloc = (tp_rtmp_alloc)GetProcAddress(handle, "RTMP_Alloc");
	rtmpInit = (tp_rtmp_init)GetProcAddress(handle, "RTMP_Init");
	rtmpSetUpUrl = (tp_rtmp_setupurl)GetProcAddress(handle, "RTMP_SetupURL");
	rtmpConnect = (tp_rtmp_connect)GetProcAddress(handle, "RTMP_Connect");
	rtmpConnectStream = (tp_rtmp_connectstream)GetProcAddress(handle, "RTMP_ConnectStream");
	rtmpRead = (tp_rtmp_read)GetProcAddress(handle, "RTMP_Read");
	rtmpClose = (tp_rtmp_close)GetProcAddress(handle, "RTMP_Close");
	rtmpSetLog = (tp_rtmp_setlog_output)GetProcAddress(handle, "RTMP_LogSetOutput");
	if (rtmpAlloc == NULL || rtmpInit == NULL || rtmpSetUpUrl == NULL || rtmpConnect == NULL
		|| rtmpConnectStream == NULL || rtmpRead == NULL || rtmpClose == NULL || rtmpSetLog == NULL)
	{
		int error = GetLastError();
		printf("Get proc address failed, error: \n", error);
		return false;
	}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{

	//1.启动SOCKET库，版本为2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 0);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		int error = GetLastError();
		printf("Socket2.0初始化失败，error: %d!\n", error);
		system("pause");
		return 0;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return 0;
	}


	if (!GetFunction())
	{
		system("pause");
		return 0;
	}
	
	//rtmpSetLog(flog);
	RTMP *pSession = rtmpAlloc();
	rtmpInit(pSession);


	int iRet = rtmpSetUpUrl(pSession, "rtmp://10.1.10.246:1935/live/livestream");
	//int iRet = rtmpSetUpUrl(pSession, "http://218.75.139.38:1935/live/1146_02_1");
	if (iRet <= 0)
	{
		printf("Setup url failed\n");
		system("pause");
		return 0;
	}
	RTMPPacket *pPack = new RTMPPacket;
	iRet = rtmpConnect(pSession, 0);
	if (iRet <= 0)
	{
		printf("connect to sever failed\n");
		system("pause");
		return 0;
	}
	iRet = rtmpConnectStream(pSession, 0);
	if (iRet <= 0)
	{
		printf("connect to stream\n");
		system("pause");
		return 0;
	}

	FILE *f = fopen("http_rtmp.flv", "wb");
	if (!f)
	{
		printf("open flv file failed!\n");
		system("pause");
		return 0;
	}
	char *buf = new char [1024];
	unsigned int len = 1024, readlen = 0;
	readlen = rtmpRead(pSession, buf, 1024);
	while (readlen > 0)
	{
		fwrite(buf, 1, readlen, f);
		readlen = rtmpRead(pSession, buf, 1024);
	}

	rtmpClose(pSession);
	fclose(f);
	return 0;
}

