#ifndef _HTTP_SERVER_
#define _HTTP_SERVER_
#include "microhttpd.h"
#include <map>
#include "streamServer.h"
#ifdef _DEBUG
#pragma comment(lib, "libmicrohttpd_d.lib")
#else
#pragma comment(lib, "libmicrohttpd.lib")
#endif


namespace stream_svr{

	static int
		new_connection(void *cls, struct MHD_Connection *connection,
		const char *url, const char *method,
		const char *version, const char *upload_data,
		size_t *upload_data_size, void **con_cls);

	class httpServer
	{
	public:
		httpServer();
		httpServer(int port);
		~httpServer();
		int start();
		void stop();

		void addStream(struct MHD_Connection *);
	private:

		MHD_Daemon *_demonServer;
		std::map<int, streamServer *> _streamClients;
		int streamKey;

		int _port;

	};
}











#endif

