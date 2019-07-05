## learning video stream

 ### rtspClient: 
  &mesp;&emsp; a rtsp client based on libEasyRTSPClient.dll
 ### librtmp: 
 &emsp;&emsp;  a rtmp client based on librtmp
 ### httpflv-client:  
 &meps;&emsp; a http client based on libcurl which receive package from http server, the package data are wrap in rtmp format, so called:http-flv
 ### httpflv-client-server: 
 &emsp;&emsp; a http-server based on libmicrohttpd, when a request come in,start a http-client to receive data from another http-flv server and send
 every package to the comming in client