#include "stdafx.h"
#include "streamServer.h"
#include <thread>
#include "../curl/curl.h"

namespace stream_svr{

	size_t write_callback(char *buffer, size_t size, size_t nitems, void *userdata)
	{
		MHD_ConnectionInfo *conn = (MHD_ConnectionInfo *)userdata;
		send(conn->connect_fd, buffer, size * nitems, 0);
		return size * nitems;
	}

	streamServer::streamServer()
	{

	}

	streamServer::streamServer(struct MHD_Connection *connection)
	{
		_conn = connection;
		//StartReceiveAndPublish();
		StartPublish();
	}

	streamServer::~streamServer()
	{

	}

	void streamServer::StartPublish()
	{
		// read file 
		// send all the file data to client once 

		// or   send file data to client byte by byte
		FILE *f = fopen("../test.flv", "rb");
		if (!f)
		{
			printf("open flv file error\n");
			return;
		}

		fseek(f, 0, SEEK_END);
		unsigned long length = ftell(f);
		rewind(f);
		char *pbuf = new char[length];
		unsigned long relen = fread(pbuf, 1, length, f);
		if (relen != length)
		{
			printf("read flv file failed!\n");
			return ;
		}
		fclose(f);

		const MHD_ConnectionInfo *pinfo = MHD_get_connection_info(_conn, MHD_CONNECTION_INFO_CONNECTION_FD);

		char header[1024] = {0};
		int len = strlen("HTTP/1.0 200 OK\r\nContent-Type:application/octet-stream\r\n\r\n");
		sprintf(header, "HTTP/1.0 200 OK\r\nContent-Type:application/octet-stream\r\n\r\n", len);

		int i = send(pinfo->connect_fd, header, len, 0);
		if (i == SOCKET_ERROR)
		{
			int err = GetLastError();
			printf("send error, error code: %d\n", err);
		}

		int packlen = 2048;
		while (true)
		{
			int i = 0;
			for (; i * packlen + packlen < length; i++)
			{
				send(pinfo->connect_fd, pbuf + i * packlen, packlen, 0);
			}

			send(pinfo->connect_fd, pbuf + i * packlen, length - i * packlen, 0);
			Sleep(5);
		}
		
	return;
		
	}

	void streamServer::StartReceiveAndPublish()
	{

		// start a client
		// received data from a http-flv server
		// resend every data from http-flv server to client

		const MHD_ConnectionInfo *pinfo = MHD_get_connection_info(_conn, MHD_CONNECTION_INFO_CONNECTION_FD);

		char header[1024] = { 0 };
		int len = strlen("HTTP/1.0 200 OK\r\nContent-Type:application/octet-stream\r\n\r\n");
		sprintf(header, "HTTP/1.0 200 OK\r\nContent-Type:application/octet-stream\r\n\r\n", len);

		int i = send(pinfo->connect_fd, header, len, 0);

		CURL *pHandle = curl_easy_init();
		CURLcode res = curl_easy_setopt(pHandle, CURLOPT_URL, "http://10.1.10.246:8080/live/livestream.flv");
		if (res != CURLE_OK)
		{
			printf("set curl opt failed\n");
			system("pause");
			return ;
		}
		res = curl_easy_setopt(pHandle, CURLOPT_WRITEFUNCTION, write_callback);
		if (res != CURLE_OK)
		{
			printf("set curl opt failed\n");
			system("pause");
			return ;
		}

		res = curl_easy_setopt(pHandle, CURLOPT_WRITEDATA, pinfo);
		if (res != CURLE_OK)
		{
			printf("set curl opt failed\n");
			system("pause");
			return ;
		}
		res = curl_easy_perform(pHandle);
		if (res != CURLE_OK)
		{
			printf("perform request failed\n");
			system("pause");
			return ;
		}

		curl_easy_cleanup(pHandle);
	}



};


