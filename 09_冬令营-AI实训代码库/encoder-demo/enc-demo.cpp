/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * AS IS BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * Copyright (c) 2018, Open AI Lab
 * 
 *
 */

#include <v4l2Cam.hpp>
#include <rockchip_rga.hpp>
#include <rockchip_mpp.hpp>
#include <rtspclient.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "openai_io.hpp"
#include "fastcv.hpp"

using namespace fcv;
using namespace std;

#define BUFFER_COUNT	8

#if 0
#define demo_dbg(fmt, ...) printf(fmt, ## __VA_ARGS__)
#else
#define demo_dbg(fmt, ...)
#endif

typedef struct _demo_data {
	v4l2Cam *cam;
	rockchip_rga *rga;
	mpp_encoder *enc;
	mpp_decoder *dec;

	char dev[32];
	CameraType type;
	struct rga_format src_fmt;
	struct rga_format dst_fmt;
	struct rga_crop crop;
	RgaRotate rotate;
	int enLive;

	RgaBuffer *buffers[BUFFER_COUNT];
	Mat mat;
	FILE *fileH264;
} DemoData;


const string winname = "test";
static void usage(char **argv)
{
	demo_dbg(
		"Usage: %s [Options]\n\n"
		"Options:\n"
		"-i, --index                 Camera index\n"
		"-t, --type                   Camera type:\n"
		"                               0:   usb generic camera\n"
		"                               1:   ov9750 mipi camera\n"
		"                               2:   imx258 mipi camera\n"
		"-w, --width                  Destination images's width\n"
		"-h, --height                 Destination images's height\n"
		"-r, --rotate                 Image rotation degree:\n"
		"                               0:   none\n"
		"                               1:   vertical mirror\n"
		"                               2:   horizontal mirror\n"
		"                               90:  90 degree\n"
		"                               180: 180 degree\n"
		"                               270: 270 degree\n"
		"-c, --crop                   Crop, format: x,y,w,h\n"
		"-l, --live	                  Live display with decoder\n"
		"-q, --query                  Query cameras\n"
		"\n",
		argv[0]);
}

static const char *short_options = "i:t:w:h:r:c:lq";


static struct option long_options[] = {
    {"index", required_argument, NULL, 'i'},
    {"type", required_argument, NULL, 't'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"rotate", required_argument, NULL, 'r'},
    {"crop", required_argument, NULL, 'c'},
    {"live", no_argument, NULL, 'l'},
    {"query", no_argument, NULL, 'q'},
    {NULL, 0, NULL, 0}
};

/* convert camera type from option */
static __u32 get_camera_format(CameraType type)
{
	switch(type) {
	case CAMERA_TYPE_USB:
		return V4L2_PIX_FMT_YUYV;
	case CAMERA_TYPE_OV9750:
	case CAMERA_TYPE_IMX258:
		return V4L2_PIX_FMT_NV12;
		break;
	default:
		fprintf(stderr, "Camera type(%d) is NOT supported\n", type);
		return 0;
	}
}

/* parse crop parameters */
static void parse_crop_parameters(char *str, struct rga_crop *crop)
{
	char *p, *buf;
	const char *delims = ".,";
	__u32 v[4] = {0,0,0,0};
	int i = 0;

	buf = strdup(str);
	p = strtok(buf, delims);
	while(p != NULL) {
		v[i++] = atoi(p);
		p = strtok(NULL, delims);

		if(i >=4)
			break;
	}

	crop->x = v[0];
	crop->y = v[1];
	crop->w = v[2];
	crop->h = v[3];
}

/* query all camera devices */
static void query_cameras(void)
{
	FILE *fp = NULL;
	char path[64];
	char line[128];
	int i, count = 0;

	printf("Camera list:\n---------------\n");
	for(i = 0; i < 16; i++) {
		sprintf(path, "/sys/class/video4linux/video%d/name", i);
		fp = fopen(path, "r");
		if(fp) {
			fread(line, 16, 1, fp);
			if(strncmp(line, "rkisp10_mainpathi", 16) == 0)
				printf("%d: Mipi camera: /dev/video%d\n", count++, i);
			else if(strncmp(line, "rkisp", 5) != 0)
				printf("%d: USB camera: /dev/video%d\n", count++, i);
			fclose(fp);
		}
	}
}

/* query camera property by device index */
void query_camera_by_index(int index, int &devIndex, CameraType &type)
{
	FILE *fp = NULL;
	char path[64];
	char line[128];
	int i, count = 0;

	for(i = 0; i < 16; i++) {
		sprintf(path, "/sys/class/video4linux/video%d/name", i);
		fp = fopen(path, "r");
		if(fp) {
			fread(line, 16, 1, fp);
			if(strncmp(line, "rkisp10_mainpathi", 16) == 0)
			{
				if (count==index)
				{
					type = CAMERA_TYPE_OV9750;
					devIndex = i;
					break;
				}
				count++;
			}
			else if(strncmp(line, "rkisp", 5) != 0)
			{
				if (count==index)
				{
					type = CAMERA_TYPE_USB;
					devIndex = i;
					break;
				}
				count++;
			}
			fclose(fp);
		}
	}
}


static int camera_init_mmap(v4l2Cam *cam, const char *dev, CameraType type, struct rga_format src_fmt)
{
	int ret;

	ret = cam->init(dev, type);
	if(ret < 0)
		return ret;

	ret = cam->setfmt(src_fmt.width, src_fmt.height, src_fmt.format);
	if(ret < 0)
		return ret;

	ret = cam->reqbufs_mmap(BUFFER_COUNT);
	if(ret < 0)
		return ret;

	ret = cam->start();
	if(ret < 0)
		return ret;

	return 0;
}

static int camera_init_dma(v4l2Cam *cam, const char *dev, CameraType type, struct rga_format src_fmt, RgaBuffer **buffers)
{
	int ret, i;
	int fds[BUFFER_COUNT];

	for(i = 0; i < BUFFER_COUNT; i++)
		fds[i] = buffers[i]->fd;

	ret = cam->init(dev, type);
	if(ret < 0)
		return ret;

	ret = cam->setfmt(src_fmt.width, src_fmt.height, src_fmt.format);
	if(ret < 0)
		return ret;

	ret = cam->reqbufs_dma(BUFFER_COUNT, fds, buffers[0]->size);
	if(ret < 0)
		return ret;

	ret = cam->start();
	if(ret < 0)
		return ret;

	return 0;
}
static int camera_init_userptr(v4l2Cam *cam, const char *dev, CameraType type, struct rga_format src_fmt, RgaBuffer **buffers)
{
	int ret, i;
	void *ptr[BUFFER_COUNT];

	for(i = 0; i < BUFFER_COUNT; i++)
		ptr[i] = buffers[i]->ptr;

	ret = cam->init(dev, type);
	if(ret < 0)
		return ret;

	ret = cam->setfmt(src_fmt.width, src_fmt.height, src_fmt.format);
	if(ret < 0)
		return ret;

	ret = cam->reqbufs_userptr(BUFFER_COUNT, ptr, buffers[0]->size);
	if(ret < 0)
		return ret;

	ret = cam->start();
	if(ret < 0)
		return ret;

	return 0;
}

/* encoder init */
static int enc_init(DemoData *demo)
{
	int ret, i;
	int fds[BUFFER_COUNT];

	for(i = 0; i < BUFFER_COUNT; i++)
		fds[i] = demo->buffers[i]->fd;
	ret = demo->enc->init();
	if(ret < 0)
		return ret;

	ret = demo->enc->importFds(fds, BUFFER_COUNT);
	if(ret < 0)
		return ret;

	return 0;
}

/* Camera dequeue(capture) and encoder enqueue */
static void *onCameraCapAndEncEnqueue(void *data)
{
	DemoData *demo = (DemoData *)data;
	int ret = 0, index;


	do {
		demo->cam->dequeue(&index);
		demo_dbg("Got camera picture, index(%d:%d), and enqueue to encoder...\n", handle_count++, index);

		ret = demo->enc->enqueue(index);
		if(ret < 0)
			break;
		demo->cam->enqueue(index);

	} while(1);

	return NULL;
}

/* encoder extra data processing */
static int encoderExtraData(DemoData *demo)
{
	int ret = 0;
	EncPacket *packet;

	/* Get extra data: SPS/PPS */
	packet = demo->enc->extraData();
	fwrite(packet->data,packet->size,1,demo->fileH264);

	if (demo->enLive)
	if(packet != NULL) {
		demo_dbg("Got encoder extra data and enqueue to decoder...\n");
		ret = demo->dec->enqueue(packet->data, packet->size);
		if(ret < 0)
			return ret;
	}

	return 0;
}

/* encoder dequeue and decoder enqueue */
static void *onEncDequeueAndDecEnqueue(void *data)
{
	DemoData *demo = (DemoData *)data;
	EncPacket *packet;
	int ret;
	int fileSize = 0;

	ret = encoderExtraData(demo);
	if(ret < 0)
		return NULL;
	do {
		packet = demo->enc->dequeue();
		if(packet != NULL) {
			if(packet->is_intra) {
				ret = encoderExtraData(demo);
				if(ret < 0)
					return NULL;
			}
			fwrite(packet->data,packet->size,1,demo->fileH264);
			fileSize+=packet->size;
			demo_dbg("Got encoded data from encoder and enqueue to deocder...\n");

			while(1)
			{
			ret = demo->dec->enqueue(packet->data, packet->size);
			if(ret >=0) 
				break;
			else
				/* Decoder buffers are full, wait and retry */
				usleep(500000);
			};

			demo->enc->freePacket(packet);

		} else {
			usleep(1000);
		}
	} while(1);

	return NULL;
}

/* encoder dequeue and write video file */
int onEncDequeue(void *data)
{
	DemoData *demo = (DemoData *)data;
	EncPacket *packet;
	int ret;
	int fileSize;

	ret = encoderExtraData(demo);
	if(ret < 0)
		return -1;
	do {
		packet = demo->enc->dequeue();
		if(packet != NULL) {
			if(packet->is_intra) {
				ret = encoderExtraData(demo);
				if(ret < 0)
					return -1;
			}
			fwrite(packet->data,packet->size,1,demo->fileH264);
			fileSize+=packet->size;
			demo_dbg("Got encoded data from encoder and enqueue to deocder...\n");

			demo->enc->freePacket(packet);

		} else {
			usleep(1000);
		}
	} while(1);

	return 0;
}



static int onDecDequeueRgaConvertAndImshow(DemoData *demo)
{
	int ret;
	DecFrame *frame = NULL;
	struct rga_format src_fmt = demo->src_fmt;
	
	do {
		frame = demo->dec->dequeue();
		if(frame != 0) {
			demo_dbg("Got decoded data form decoder, convert it and imshow...\n");
			src_fmt.format = frame->v4l2Format;
			ret = demo->rga->convert(frame->data, demo->mat.data, src_fmt, demo->dst_fmt, demo->crop, demo->rotate);
			if(ret < 0)
				return ret;
			if(demo->enLive)
			{
			imshow(winname, demo->mat, NULL);
			fcv::waitKey(1);
			}
			demo->dec->freeFrame(frame);
		}else {
			usleep(1000);
		}
	} while(1);

	return 0;
}

int main(int argc, char **argv)
{
	int ret, i, c;
	pthread_t id1, id2;
    	__u32 width = 640, height = 480;
	int camIndex=0, devIndex = 0;
	CameraType optType=CAMERA_TYPE_USB,qryType=CAMERA_TYPE_USB;

	/* Demo variable definition */
	DemoData demo;
	memset(&demo, 0, sizeof(DemoData));
	sprintf(demo.dev, "%s", "/dev/video0");
	demo.type = CAMERA_TYPE_USB;
	demo.rotate = RGA_ROTATE_NONE;
	demo.enLive = 0;

	/* Dealing with options  */
	while((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch(c) {
		case 'i':
			camIndex = atoi(optarg);
			break;
		case 't':
			i = atoi(optarg);
			switch(i) {
			case 0: optType = CAMERA_TYPE_USB; break;
			case 1: optType = CAMERA_TYPE_OV9750; break;
			case 2: optType = CAMERA_TYPE_IMX258; break;
			default:
				fprintf(stderr, "Camera type(%d) is not supported\n", i);
				return -1; 
			}
			break;
		case 'w':
			width = atoi(optarg);
			break;
		case 'h':
			height = atoi(optarg);
			break;
		case 'r':
			i = atoi(optarg);
			switch(i) {
			case 0: demo.rotate = RGA_ROTATE_NONE; break;
			case 1: demo.rotate = RGA_ROTATE_VFLIP; break;
			case 2: demo.rotate = RGA_ROTATE_HFLIP; break;
			case 90: demo.rotate = RGA_ROTATE_90; break;
			case 180: demo.rotate = RGA_ROTATE_180; break;
			case 270: demo.rotate = RGA_ROTATE_270; break;
			default:
				fprintf(stderr, "Roate(%d) is not supported\n", i);
				return -1; 
			}
			break;
		case 'c':
			parse_crop_parameters(optarg, &demo.crop);
			break;
		case 'l':
			demo.enLive = 1;
			break;
		case 'q':
			query_cameras();
			return 0;
		default:
			usage(argv);
			return 0; 

		}
	}

	/* set input image resolution  */
	demo.src_fmt.width = width;
	demo.src_fmt.height = height;

	/* set camera property by index */
	query_camera_by_index(camIndex,devIndex,qryType);
	if (qryType==CAMERA_TYPE_USB)
	{
		if (optType!=CAMERA_TYPE_USB)
			printf ("Camera%d is detected as USB camera. Ignore option setting!\n", camIndex);
		demo.type = qryType;
	}
	else if (qryType==CAMERA_TYPE_OV9750)
	{
		if (optType==CAMERA_TYPE_USB)
			printf ("Camera%d is detected as MIPI camera. Ignore option setting!\n", camIndex);
		demo.type = optType;
	}
	sprintf(demo.dev, "/dev/video%d", devIndex);
	if (devIndex==0)
		printf("default device /dev/video0 is used.\n");
	demo.src_fmt.format = get_camera_format(demo.type);

	/* set encoder parameters */
	demo.dst_fmt.width = width;
	demo.dst_fmt.height = height;
	demo.dst_fmt.format = V4L2_PIX_FMT_RGB24;
	demo.fileH264 = fopen("./test.264", "w+");

	demo_dbg("dev %s,type %d, width %u, height %u, rotate %u, crop [%u, %u, %u, %u]\n",
		demo.dev, demo.type, width, height, demo.rotate, demo.crop.x, demo.crop.y, demo.crop.w, demo.crop.h);
		

	/* Init rga and alloc dma buffers */
	demo.rga = new rockchip_rga();
	ret = demo.rga->init();
	if(ret < 0)
		{
		goto exit1;
		}
	for(i = 0; i < BUFFER_COUNT; i++) {
		demo.buffers[i] = demo.rga->alloc(demo.src_fmt.format, demo.src_fmt.width, demo.src_fmt.height);
		demo_dbg("Drm alloc: %d, %p, %lu\n", i, demo.buffers[i]->ptr, demo.buffers[i]->size);
	}


	/* Init mpp encoder */
	demo.enc = new mpp_encoder(demo.src_fmt.format, demo.src_fmt.width, demo.src_fmt.height, demo.buffers[0]->size);
	ret = enc_init(&demo);
	if(ret < 0)
	{
		goto exit2;
	}

	/* Init v4l2 camera */
	demo.cam = new v4l2Cam();
	ret = camera_init_dma(demo.cam, demo.dev, demo.type, demo.src_fmt, demo.buffers);
	if(ret < 0)
	{
		goto exit3;

	}

	/* Init mpp decoder under live mode */
	if (demo.enLive)
	{
		demo.dec = new mpp_decoder();
		ret = demo.dec->init(DECODE_TYPE_H264);
		if(ret < 0)
		{
		goto exit4;
		}
		fcv::namedWindow(winname);
		demo.mat.create(cv::Size(RGA_ALIGN(demo.dst_fmt.width, 16), RGA_ALIGN(demo.dst_fmt.height, 16)), CV_8UC3);

	}

	/* Create thread for camera capture and encoder enqueue */
	ret = pthread_create(&id1, NULL, onCameraCapAndEncEnqueue, &demo);
	if(ret < 0) {
		fprintf(stderr, "Create pthread1 failed\n");
		goto exit4;
	}

	/* Create thread for encoder dequeue and decoder enqueue under live mode */
	if (demo.enLive)
	{
	ret = pthread_create(&id2, NULL, onEncDequeueAndDecEnqueue, &demo);
	if(ret < 0) {
		fprintf(stderr, "Create pthread2 failed\n");
		goto exit4;
	}
	}

	do {
		/* Decoder dequeue , rga convert and imshow under live mode */
		if (demo.enLive)
		{
		ret = onDecDequeueRgaConvertAndImshow(&demo);
		if(ret < 0)
			break;
		}
		/* Encoder dequeue */
		else
		{
			ret = onEncDequeue(&demo);
			if(ret < 0)
				break;
		}			
	sleep(30000);
	} while(1);

	/* release memory */
exit4:	
	if (demo.enLive)
	{
		delete demo.dec;
	}

exit3:
	delete demo.cam;


exit2:	
	delete demo.enc;
	for(i = 0; i < BUFFER_COUNT; i++)
	{
		demo.rga->free(demo.buffers[i]);
	}
	
exit1:	
	
	delete demo.rga;
	fclose(demo.fileH264);

	return ret;
}
