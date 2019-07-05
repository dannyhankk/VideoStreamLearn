#ifndef _STREAM_SERVER_
#define _STREAM_SERVER_
#include "microhttpd.h"


#ifdef _DEBUG
#pragma comment(lib, "libmicrohttpd_d.lib")
#else
#pragma comment(lib, "libmicrohttpd.lib")
#endif

namespace stream_svr{

	class streamServer
	{
	public:
		streamServer();
		streamServer(struct MHD_Connection *);
		~streamServer();
		void StartPublish();

		void StartReceiveAndPublish();
	private:

		MHD_Connection * _conn;
	};
};


#endif