//#include <stdio.h>
//#include <GLFW/glfw3.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <inttypes.h>
}

bool loadFrame(const char* fileName, int* width_out, int* height_out, unsigned char** data_out)
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
		avCodecParameters = stream->codecpar;
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

	AVCodecContext* avCodecContext = avcodec_alloc_context3(avCodec);
	if (!avCodecContext) {
		printf("Could not create AVCodecContext\n");
		return false;
	}

	if (avcodec_parameters_to_context(avCodecContext, avCodecParameters) < 0) {
		printf("Could not initialize AVCodecContext\n");
		return false;
	}
	if (avcodec_open2(avCodecContext, avCodec, nullptr) < 0) {
		printf("Could not open codec\n");
		return false;
	}



	AVFrame* avFrame = av_frame_alloc();
	if (!avFrame) {
		printf("Could not allocate AVFrame\n");
		return false;
	}
	AVPacket* avPacket = av_packet_alloc();
	if (!avPacket) {
		printf("Could not allocate AVPacket\n");
		return false;
	}


	while (av_read_frame(avFormatContext, avPacket) >= 0) {
		if (avPacket->stream_index != videoStreamIndex) {
			continue;
		}
		int response = avcodec_send_packet(avCodecContext, avPacket);
		if (response < 0) {
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(response, errbuf, AV_ERROR_MAX_STRING_SIZE);
			printf("Failed to decode packet: %s\n", errbuf);
			return false;
		}
		response = avcodec_receive_frame(avCodecContext, avFrame);
		if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
			continue;
		}
		else if (response < 0) {
			char errbuf[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(response, errbuf, AV_ERROR_MAX_STRING_SIZE);
			printf("Failed to decode packet: %s\n", errbuf);
			return false;
		}

		av_packet_unref(avPacket);
		break;
	}

	unsigned char* data = new unsigned char[avFrame->width * avFrame->height * 3];
	for (size_t x = 0; x < avFrame->width; x++)
	{
		for (size_t y = 0; y < avFrame->height; y++)
		{
			data[y * avFrame->width * 3 + x * 3 + 0] = avFrame->data[0][y * avFrame->linesize[0] + x];;
			data[y * avFrame->width * 3 + x * 3 + 1] = avFrame->data[0][y * avFrame->linesize[0] + x];;
			data[y * avFrame->width * 3 + x * 3 + 2] = avFrame->data[0][y * avFrame->linesize[0] + x];;
		}
	}
	*width_out = avFrame->width;
	*height_out = avFrame->height;
	*data_out = data;



	avformat_close_input(&avFormatContext);
	avformat_free_context(avFormatContext);
	av_frame_free(&avFrame);
	av_packet_free(&avPacket);
	avcodec_free_context(&avCodecContext);

	return true;
}