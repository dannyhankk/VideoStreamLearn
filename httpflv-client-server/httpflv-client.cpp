// httpflv-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "curl/curl.h"
#include <stdio.h>
#include <stdlib.h>
#include "microhttp/httpServer.h"

using namespace stream_svr;

size_t write_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	fwrite(buffer, size, nitems, (FILE *)userdata);
	return size * nitems;
}
int _tmain(int argc, _TCHAR* argv[])
{

	httpServer *pHttp = new httpServer(8090);

	pHttp->start();

	system("pause");
	return 0;
}

