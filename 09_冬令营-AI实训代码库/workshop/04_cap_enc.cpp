#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/videodev2.h>

#include "openai_io.hpp"
#include "fastcv.hpp"
#include "fastcv.hpp"

#include <rockchip/rockchip_mpp.h>

using namespace fcv;
using namespace std;

extern "C" {
#include <rockchip/rockchip_rga.h>
}
#include <rockchip/rockchip_isp.h>

#define BUFFER_COUNT 8

const string wintitle = "mipi-camera";
enum CAM_TYPE {
    CAM_OV9750 = 0,
    CAM_IMX258,
    CAM_MAX,
};

typedef struct{
    int fd;
    void *start;
    size_t length;
}Buffer_t;
typedef struct{
    int runflag;
    int fd;
    int cam_type;
    void *rkengine;
    RockchipRga *rga;
	MppEncoder *enc;
	MppDecoder *dec;

    __u32 cap_width;
    __u32 cap_height;
    __u32 cap_format;       // pixel format

    __u32 disp_width;
    __u32 disp_height;
    __u32 disp_format;       // pixel format

    __u32 enc_width;
    __u32 enc_height;
    __u32 enc_format;       // pixel format
    __u32 enc_size;       // pixel format

    RgaRotate rotate;
    __u32 cropx;
    __u32 cropy;
    __u32 cropw;
    __u32 croph;

	__u32 memory;
    int nbuffer;
    Buffer_t Cap_buf[BUFFER_COUNT];
    RgaBuffer *Enc_buf[BUFFER_COUNT];
    Mat image;
    

    __u32 start;
    __u32 time[5];
    __u32 timeall[5];
    __u32 count;

    FILE *fileH264;
    __u32 filesize;
}Workshop_t;
Workshop_t tDemo;
Workshop_t *demo=&tDemo;

__u32 GetTime()
{
    struct timeval t0;
    gettimeofday(&t0, NULL);

    return (t0.tv_sec*10000 + t0.tv_usec/100);
}

int v4l2_init(char *v4l2_dev, char *isp_dev)
{
    int ret;

    // Open device driver
    tDemo.fd = open(v4l2_dev, O_RDWR, 0);
    if(tDemo.fd < 0) {
        printf("Open dev %s failed\n", v4l2_dev);
        return tDemo.fd;
    }

#if 1
    // set capture format
    struct v4l2_format fmt;

    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = tDemo.cap_width;
    fmt.fmt.pix.height = tDemo.cap_height;
    fmt.fmt.pix.pixelformat = tDemo.cap_format;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ret = ioctl(tDemo.fd, VIDIOC_S_FMT, &fmt);
    if(ret < 0) {
        printf("Set format failed\n");
        return ret;
    }
#endif
#if 1
    /* Call rockchip_isp interface */
    if(tDemo.cam_type == CAM_OV9750) {
        ret = rkisp_start(tDemo.rkengine, tDemo.fd, isp_dev, "/etc/cam_iq_ov9750.xml");
        printf("Open isp_dev %s\n", isp_dev);
    }
#endif
#if 1
    // request buffer
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (tDemo.memory == V4L2_MEMORY_DMABUF)
        req.memory = V4L2_MEMORY_DMABUF;
    else
        req.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(tDemo.fd, VIDIOC_REQBUFS, &req);
    if(ret < 0) {
        printf("Request buffers failed\n");
        return ret;
    }
#endif
#if 1       // only under mmap mode need to mapping
    if (tDemo.memory == V4L2_MEMORY_MMAP)
        // mapping buffer to app
        for(int i = 0; i < BUFFER_COUNT; i++) {
            struct v4l2_buffer buf;
            memset(&buf, 0, sizeof(buf));
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;
            // get buffer info
            ret = ioctl(tDemo.fd, VIDIOC_QUERYBUF, &buf);
            if(ret < 0) {
                printf("Query buffers failed\n");
                return ret;
            }
            tDemo.Cap_buf[i].length = buf.length;
            tDemo.Cap_buf[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, tDemo.fd, buf.m.offset);
            if(MAP_FAILED == tDemo.Cap_buf[i].start) {
                printf("mmap buffers failed\n");
                return ret;
            }
        }
    tDemo.enc_size = tDemo.Cap_buf[0].length;
#endif
#if 1      
    for(int i = 0; i < BUFFER_COUNT; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.index = i;
        if (tDemo.memory ==  V4L2_MEMORY_MMAP){
            buf.memory = V4L2_MEMORY_MMAP;
        }else if (tDemo.memory == V4L2_MEMORY_DMABUF){
            buf.memory = V4L2_MEMORY_DMABUF;
            buf.m.fd  = tDemo.Enc_buf[i]->fd;
            buf.length = tDemo.Enc_buf[i]->size;
            printf("QBUF %d %d %d\n",i,buf.m.fd,buf.length);
        }
        ret = ioctl(tDemo.fd, VIDIOC_QBUF, &buf);
        if(ret < 0) {
            printf("Queue buffers failed\n");
            return ret;
        }
    }
#endif
#if 1
    int type;
    // start capture 
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(tDemo.fd, VIDIOC_STREAMON, &type);
    if(ret < 0) {
        printf("Stream on failed\n");
        return ret;
    }
#endif
    return 0;
}

int v4l2_uninit()
{
    int i;
    int ret;
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(tDemo.fd, VIDIOC_STREAMOFF, &type);
    if(ret < 0) {
        printf("Stream off failed\n");
        return ret;
    }

    for(i = 0; i < BUFFER_COUNT; i++) {
        munmap(tDemo.Cap_buf[i].start, tDemo.Cap_buf[i].length);
    }
    rkisp_stop(tDemo.rkengine);
    close(tDemo.fd);

    return 0;
}
int rga_init()
{
    tDemo.rga = RgaCreate();
    if(tDemo.rga == NULL) {
        printf("Rga Create Fail\n");
        return -1;
    }
    return 0;
}
int rga_uninit()
{
    if(tDemo.rga != NULL)
        RgaDestroy(tDemo.rga);
    return 0;
}
int disp_init()
{
    /* Window -- create */
    fcv::namedWindow(wintitle);
    fcv::moveWindow(wintitle, tDemo.disp_width, tDemo.disp_height);
    tDemo.image.create(cv::Size(RGA_ALIGN(tDemo.disp_width, 16), RGA_ALIGN(tDemo.disp_height, 16)), CV_8UC3);
    return 0;
}

int enc_init()
{
    struct _EncCtx ctx;

	ctx.width = tDemo.enc_width;
	ctx.height = tDemo.enc_height;
	ctx.v4l2Format = tDemo.enc_format;
	ctx.size = tDemo.enc_size;
	ctx.fps = 30;
	ctx.gop = 60;
	ctx.mode = ENCODE_RC_MODE_CBR;
	ctx.quality = ENCODE_QUALITY_MEDIUM;
    printf("enc_init: w=%d h=%d f=%d size=%d\n",tDemo.enc_width,tDemo.enc_height,tDemo.enc_format,tDemo.enc_size);
 	tDemo.enc = MppEncoderCreate(ctx);
	if(!tDemo.enc) {
		fprintf(stderr, "Create mpp encoder failed\n");
		return -EIO;
	}
    return 0;
}
int enc_uninit()
{
    if(tDemo.enc)
		MppEncoderDestroy(tDemo.enc);
    return 0;
}
int enc_frameque_init()
{
    int i;    
    RgaBuffer *buf;
	for(i = 0; i < BUFFER_COUNT; i++) {
		buf = tDemo.rga->ops->allocDmaBuffer(tDemo.rga,tDemo.cap_format,tDemo.cap_width,tDemo.cap_height);
        tDemo.enc->ops->importBufferFd(tDemo.enc,buf->fd,i);
		printf("Drm alloc: %d,%d, %p, %lu\n", i, buf->fd,buf->ptr, buf->size);
        tDemo.Enc_buf[i] = buf;
	}
    return 0;
}

int Cap_process()
{
    int ret,index;
    struct v4l2_buffer buf;
    RockchipRga *rga = tDemo.rga;

    tDemo.start = GetTime();
    tDemo.count++;
#if 1
    // get the buffer from queue
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (tDemo.memory==V4L2_MEMORY_MMAP)
        buf.memory = V4L2_MEMORY_MMAP;
    else
        buf.memory = V4L2_MEMORY_DMABUF;
    ret = ioctl(tDemo.fd, VIDIOC_DQBUF, &buf);
    if(ret < 0) {
        printf("Dequeue buffer failed, ret = %d\n", ret);
        return ret;
    }
    tDemo.time[0] = GetTime() - tDemo.start;
    index = buf.index;
#endif
    if (tDemo.memory==V4L2_MEMORY_MMAP){
        // transfor using rga
        rga->ops->initCtx(rga);
        rga->ops->setSrcBufferPtr(tDemo.rga, tDemo.Cap_buf[index].start);
        rga->ops->setDstBufferPtr(tDemo.rga, tDemo.image.data);

        rga->ops->setSrcFormat(tDemo.rga, tDemo.cap_format, tDemo.cap_width, tDemo.cap_height);
        rga->ops->setDstFormat(tDemo.rga, tDemo.disp_format, tDemo.disp_width, tDemo.disp_height);

        rga->ops->setRotate(tDemo.rga, tDemo.rotate);
        if(tDemo.cropw > 0 && tDemo.croph > 0) {
            rga->ops->setSrcCrop(tDemo.rga, tDemo.cropx, tDemo.cropy, tDemo.cropw, tDemo.croph);
            rga->ops->setDstCrop(tDemo.rga, 0, 0, tDemo.disp_width, tDemo.disp_height);
        }
        rga->ops->go(tDemo.rga);
        tDemo.time[1] = GetTime() - tDemo.start;

        rga->ops->initCtx(rga);
        rga->ops->setSrcBufferPtr(tDemo.rga, tDemo.Cap_buf[index].start);
        rga->ops->setDstBufferPtr(tDemo.rga, tDemo.Enc_buf[index]->ptr);
        rga->ops->setSrcFormat(tDemo.rga, tDemo.cap_format, tDemo.cap_width, tDemo.cap_height);
        rga->ops->setDstFormat(tDemo.rga, tDemo.enc_format, tDemo.enc_width, tDemo.enc_height);
        rga->ops->go(tDemo.rga);
        ret = tDemo.enc->ops->enqueue(tDemo.enc,index);
        //printf("Cap->Enc %d\n",index);
        if(ret < 0)
            return -1;
#if 1
        // put the buffer back into capture queue
        ret = ioctl(tDemo.fd, VIDIOC_QBUF, &buf);
        if(ret < 0) {
            printf("Eequeue buffer failed\n");
            return ret;
        }
        tDemo.time[2] = GetTime() - tDemo.start;
#endif
#if 1
        /* Window -- drawing frame */
        fcv::imshow(wintitle, tDemo.image, NULL);
        fcv::waitKey(1);
    }
#endif
    tDemo.time[3] = GetTime() - tDemo.start;

    tDemo.time[4] = tDemo.time[3];
    tDemo.time[3] -= tDemo.time[2];
    tDemo.time[2] -= tDemo.time[1];
    tDemo.time[1] -= tDemo.time[0];
    for(int i=0;i<5;i++){
        tDemo.timeall[i] += tDemo.time[i];
    }

    return 0;
}

void *Capture_Task(void*)
{
    printf("Enter Capture task\n");
    while(1){
        if (tDemo.runflag)
            Cap_process();
    }
}
void *Encoder_Task(void *)
{
	EncPacket *packet;
	EncPacket *packet_I;

    printf("Enter Encoder data task\n");

    tDemo.fileH264 = fopen("test.264","wb+");
    if (tDemo.fileH264==NULL){
        printf("Cannot create file\n");
        return NULL;
    }
    printf("Open %s file for write stream\n","test.264");
    tDemo.filesize = 0;

	/* Get extra data: SPS/PPS */
	packet = tDemo.enc->ops->getExtraData(tDemo.enc);
	fwrite(packet->data,packet->size,1,tDemo.fileH264);
	tDemo.filesize += packet->size;
	while(1) {
        if (tDemo.runflag==0) break;
		packet = tDemo.enc->ops->dequeue(tDemo.enc);
		if(packet != NULL) {
			if(packet->is_intra) {
                printf("I frame %d\n",tDemo.count);
                packet_I = tDemo.enc->ops->getExtraData(tDemo.enc);
                fwrite(packet_I->data,packet_I->size,1,tDemo.fileH264);
    			tDemo.filesize += packet_I->size;
			}
            fwrite(packet->data,packet->size,1,tDemo.fileH264);
        	tDemo.filesize += packet->size;

            tDemo.enc->ops->freePacket(packet);
		} else {
			usleep(10000);
		}
	}
    fclose(tDemo.fileH264);
    return 0;
}
int main(int argc, char *argv[])
{
    int ret;
    char v4l2_dev[32];
    char isp_dev[32];
    char cmd;
	pthread_t pid1;

#if 1
    tDemo.cam_type = CAM_OV9750;
    tDemo.cap_width  = 720;
    tDemo.cap_height = 480;
    tDemo.cap_format = V4L2_PIX_FMT_NV12;
#else
    tDemo.cap_width  = 640;
    tDemo.cap_height = 480;
    tDemo.cam_type = CAM_MAX;
    tDemo.cap_format = V4L2_PIX_FMT_YUYV;
#endif
    tDemo.disp_width  = 640;
    tDemo.disp_height = 480;
    tDemo.disp_format = V4L2_PIX_FMT_RGB24;

    tDemo.enc_width = tDemo.cap_width;
    tDemo.enc_height= tDemo.cap_height;
    tDemo.enc_format= tDemo.cap_format;
    //tDemo.enc_size        // init in capture init.
    //tDemo.rotate = RGA_ROTATE_270;
#if 0
    tDemo.cropx = 32;
    tDemo.cropy = 32;
    tDemo.cropw = 320;
    tDemo.croph = 240;
#endif
    tDemo.count = 0;
    for(int i=0;i<5;i++)
        tDemo.timeall[i] = 0;

    //tDemo.memory = V4L2_MEMORY_DMABUF;
    tDemo.memory = V4L2_MEMORY_MMAP;
    printf("init rga\n");
    rga_init();
    printf("init encoder\n");
	enc_init();
    printf("init encoder buffer\n");
    enc_frameque_init();

    sprintf(v4l2_dev,"%s","/dev/video2");
    sprintf(isp_dev,"%s","/dev/video1");
    ret = v4l2_init(v4l2_dev,isp_dev);
    if (ret<0){
        printf("V4l2 init fail\n");
        return 0;
    }
    printf("init display\n");
    disp_init();

    tDemo.runflag = 1;
	ret = pthread_create(&pid1, NULL, Capture_Task, NULL);
	ret = pthread_create(&pid1, NULL, Encoder_Task, NULL);

    printf("Enter main loop\n");
    while(1){
        cmd = getchar();
        if (cmd=='q') {
            tDemo.runflag = 0;
            break;
        }
        printf("FRM:%4d CAPQ:%3d %3d RGA:%3d %3d CAPDQ:%3d %3d DISP:%3d %3d ALL:%3d %3d\n",tDemo.count,
            tDemo.time[0],tDemo.timeall[0]/tDemo.count,
            tDemo.time[1],tDemo.timeall[1]/tDemo.count,
            tDemo.time[2],tDemo.timeall[2]/tDemo.count,
            tDemo.time[3],tDemo.timeall[3]/tDemo.count,
            tDemo.time[4],tDemo.timeall[4]/tDemo.count);
    }
    enc_uninit();
    rga_uninit();
    v4l2_uninit();
}
