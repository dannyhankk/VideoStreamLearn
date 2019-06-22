// rtspClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "Include/EasyRTSPClientAPI.h"
#include "Include/EasyTypes.h"


#define KEY "6D75724D7A4969576B5A7541725370636F3956524575314659584E35556C525455454E73615756756443356C65475570567778576F5036532F69426C59584E35"

Easy_Handle fRTSPHandle = 0;
char *fRTSPUrl = "rtsp://997_00_1:997_00_1@218.75.139.38:554/997_00_1";

FILE *f264 = NULL;
char *fileName = "test.264";

int Easy_APICALL __RTSPClientCallBack(int _chid, void *_chPtr, int _frameType, char *_pBuf, EASY_FRAME_INFO* _frameInfo)
{
	if (_frameType == EASY_SDK_VIDEO_FRAME_FLAG)
	{
		if (_frameInfo->codec == EASY_SDK_VIDEO_CODEC_H264)
		{
			fwrite(_pBuf, 1, _frameInfo->length, f264);


			//for specific I frame, p frame, SPS, PPS
			/*
			|SPS+PPS+IDR|
			|---------------------|----------------|-------------------------------------|
			|                     |                |                                     |
			0-----------------reserved1--------reserved2-------------------------------length
			*/
			if (_frameInfo->type == EASY_SDK_VIDEO_FRAME_I)
			{
				char sps[2048] = { 0 };
				char pps[2048] = { 0 };
				char* IFrame = NULL;
				unsigned int spsLen, ppsLen, iFrameLen = 0;

				spsLen = _frameInfo->reserved1;                          // sps
				ppsLen = _frameInfo->reserved2 - _frameInfo->reserved1;  // pps
				iFrameLen = _frameInfo->length - spsLen - ppsLen;        // IDR

				memcpy(sps, _pBuf, spsLen);			//SPS
				memcpy(pps, _pBuf + spsLen, ppsLen);	//PPS
				IFrame = _pBuf + spsLen + ppsLen;	//IDR
			}
			else if (_frameInfo->type == EASY_SDK_VIDEO_FRAME_P)
			{
				// process p frame
			}
		}
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	EasyRTSP_Activate(KEY);

	f264 = fopen(fileName, "wb");
	if (NULL == f264)
	{
		printf("open 264 file failed!\n");
		system("pause");
		return 0;
	}

	EasyRTSP_Init(&fRTSPHandle);
	if (NULL == fRTSPHandle)
	{
		printf("Init easy rtsp client handle failed!\n");
		system("pause");
		return 0;

	}

	EasyRTSP_SetCallback(fRTSPHandle, __RTSPClientCallBack);

	unsigned int mediaType = EASY_SDK_VIDEO_FRAME_FLAG | EASY_SDK_AUDIO_FRAME_FLAG;

	EasyRTSP_OpenStream(fRTSPHandle, 0, fRTSPUrl, EASY_RTP_OVER_TCP, mediaType, NULL, NULL, NULL, 1000, 0, 0x01, 3);

	system("pause");
	EasyRTSP_CloseStream(fRTSPHandle);
	EasyRTSP_Deinit(&fRTSPHandle);
	fRTSPHandle = NULL;
	return 0;
}

