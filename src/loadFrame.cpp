//#include <stdio.h>
//#include <GLFW/glfw3.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <inttypes.h>
}

bool loadFrame(const char* fileName, int* width, int* height, unsigned char** data)
{
	AVFormatContext* avFormatContext = avformat_alloc_context();
	if (!avFormatContext) {
		printf("Could not create AVFormatContext\n");
		return false;
	}
	if (avformat_open_input(&avFormatContext, fileName, nullptr, nullptr) != 0) {
		printf("Could not open video file\n");
		return false;
	}

	// Find first valid Video Stream inside the file
	int videoStreamIndex = -1;
	AVCodecParameters* avCodecParameters = nullptr;
	const AVCodec* avCodec = nullptr;
	for (size_t i = 0; i < avFormatContext->nb_streams; i++)
	{
		auto stream = avFormatContext->streams[i];
		AVCodecParameters* avCodecParameters = stream->codecpar;
		avCodec = avcodec_find_decoder(avCodecParameters->codec_id);

		if (!avCodec) {
			continue;
		}
		if (avCodec->type == AVMEDIA_TYPE_VIDEO) {
			videoStreamIndex = i;
			break;
		}
	}

	if (videoStreamIndex == -1) {
		printf("Could not find valid video stream inside file\n");
	}

	return true;
}