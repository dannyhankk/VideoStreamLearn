// httpflv-client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "curl/curl.h"
#include <stdio.h>
#include <stdlib.h>


size_t write_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	fwrite(buffer, size, nitems, (FILE *)userdata);
	return size * nitems;
}
int _tmain(int argc, _TCHAR* argv[])
{
	FILE * pf = fopen("http-flv.flv", "wb");
	if (!pf)
	{
		printf("open flv file failed\n");
		system("pause");
		return 0;
	}
	CURL *pHandle = curl_easy_init();
	CURLcode res = curl_easy_setopt(pHandle, CURLOPT_URL, "http://218.75.139.38:1935/live/989_03_1");
	if (res != CURLE_OK)
	{
		printf("set curl opt failed\n");
		system("pause");
		return 0;
	}
	res = curl_easy_setopt(pHandle, CURLOPT_WRITEFUNCTION, write_callback);
	if (res != CURLE_OK)
	{
		printf("set curl opt failed\n");
		system("pause");
		return 0;
	}

	res = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, pf);
	if (res != CURLE_OK)
	{
		printf("set curl opt failed\n");
		system("pause");
		return 0;
	}
	res = curl_easy_perform(pHandle);
	if (res != CURLE_OK)
	{
		printf("perform request failed\n");
		system("pause");
		return 0;
	}

	curl_easy_cleanup(pHandle);
	fclose(pf);
	system("pause");
	
	return 0;
}

