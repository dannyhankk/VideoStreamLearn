#include "stdafx.h"
#include "httpServer.h"
#include <iostream>
#include <map>
#include <utility>

namespace stream_svr{

	int new_connection(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls)
	{
		printf("Received request: %s\n Method: %s\n", url, method);
		httpServer *pSvr = (httpServer *)cls;
		pSvr->addStream(connection);
		return MHD_YES;
	}

	int httpServer::start()
	{
		_demonServer = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, _port, NULL, NULL,
			&new_connection, this, MHD_OPTION_END);
		if (NULL == _demonServer)
		{
			return -1;
		}

		return 0;
	}

	void httpServer::stop()
	{
		MHD_stop_daemon(_demonServer);
	}

	httpServer::httpServer():
		_demonServer(NULL)
		, streamKey(0)
		, _port(8080)
	{

	}

	httpServer::httpServer(int port):
		_port(port)
	{

	}

	httpServer::~httpServer()
	{

	}

	void httpServer::addStream(struct MHD_Connection *connection)
	{
		streamKey += 1;
		streamServer *pStream = new streamServer(connection);
		_streamClients.insert(std::make_pair(streamKey, pStream));
	}
};





