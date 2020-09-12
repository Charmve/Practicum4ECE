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

using namespace fcv;
using namespace std;

extern "C" {
#include <rockchip/rockchip_rga.h>
}
#include <rockchip/rockchip_isp.h>

#define BUF_NUM 4
const string wintitle = "mipi-camera";
enum CAM_TYPE {
    CAM_OV9750 = 0,
    CAM_IMX258,
    CAM_MAX,
};

typedef struct{
    void *start;
    size_t length;
}Buffer_t;
typedef struct{
    int fd;
    int cam_type;
    void *rkengine;
    RockchipRga *rga;

    __u32 cap_width;
    __u32 cap_height;
    __u32 cap_format;       // pixel format

    __u32 disp_width;
    __u32 disp_height;
    __u32 disp_format;       // pixel format

    RgaRotate rotate;
    __u32 cropx;
    __u32 cropy;
    __u32 cropw;
    __u32 croph;

    int nbuffer;
    Buffer_t Cap_buf[BUF_NUM];
    Mat image;

    __u32 start;
    __u32 time[5];
    __u32 timeall[5];
    __u32 count;
}Workshop_t;
Workshop_t tDemo;

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

#if 0
    // Check CAP
    struct v4l2_capability cap;

    ret = ioctl(tDemo.fd, VIDIOC_QUERYCAP, &cap);
    if(ret < 0) {
        printf("Querycap failed\n");
        return ret;
    }
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        printf("%s is NOT video capture device\n", v4l2_dev);
        return ret;
    }
    if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
        printf("%s is NOT video streaming io\n", v4l2_dev);
        return ret;
    }
#endif
#if 0
    // check format
    struct v4l2_fmtdesc fmtdesc;
    int found=0;

    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(1) {
        ret = ioctl(tDemo.fd, VIDIOC_ENUM_FMT, &fmtdesc);
        if(ret < 0)
            break;
        if(fmtdesc.pixelformat == tDemo.cap_format) {
            found = 1;
            break;
        }
        fmtdesc.index++;
    }
    if(found == 0) {
        printf("This format is NOT supported\n");
        return -1;
    }
#endif
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
    req.count = BUF_NUM;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(tDemo.fd, VIDIOC_REQBUFS, &req);
    if(ret < 0) {
        printf("Request buffers failed\n");
        return ret;
    }
#endif
#if 1
    // mapping buffer to app
    for(int i = 0; i < BUF_NUM; i++) {
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
#endif
#if 1
    for(int i = 0; i < BUF_NUM; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
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

    for(i = 0; i < BUF_NUM; i++) {
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

int process(void *dstptr)
{
    int ret;
    struct v4l2_buffer buf;
    RockchipRga *rga = tDemo.rga;

    tDemo.start = GetTime();
    tDemo.count++;
#if 1
    // get the buffer from queue
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(tDemo.fd, VIDIOC_DQBUF, &buf);
    if(ret < 0) {
        printf("Dequeue buffer failed, ret = %d\n", ret);
        return ret;
    }
    tDemo.time[0] = GetTime() - tDemo.start;
#endif
#if 1
    // transfor using rga
    rga->ops->initCtx(rga);
    rga->ops->setSrcBufferPtr(tDemo.rga, tDemo.Cap_buf[buf.index].start);
    rga->ops->setDstBufferPtr(tDemo.rga, dstptr);

    rga->ops->setSrcFormat(tDemo.rga, tDemo.cap_format, tDemo.cap_width, tDemo.cap_height);
    rga->ops->setDstFormat(tDemo.rga, tDemo.disp_format, tDemo.disp_width, tDemo.disp_height);

    rga->ops->setRotate(tDemo.rga, tDemo.rotate);
    if(tDemo.cropw > 0 && tDemo.croph > 0) {
        rga->ops->setSrcCrop(tDemo.rga, tDemo.cropx, tDemo.cropy, tDemo.cropw, tDemo.croph);
        rga->ops->setDstCrop(tDemo.rga, 0, 0, tDemo.disp_width, tDemo.disp_height);
    }
    rga->ops->go(tDemo.rga);
    tDemo.time[1] = GetTime() - tDemo.start;
#endif
#if 1
    // put the buffer into queue
    ret = ioctl(tDemo.fd, VIDIOC_QBUF, &buf);
    if(ret < 0) {
        printf("Eequeue buffer failed\n");
        return ret;
    }
    tDemo.time[2] = GetTime() - tDemo.start;
#endif
    /* Window -- drawing frame */
    fcv::imshow(wintitle, tDemo.image, NULL);
    fcv::waitKey(1);
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
int main(int argc, char *argv[])
{
    int ret;
    char v4l2_dev[32];
    char isp_dev[32];

#if 0
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

    sprintf(v4l2_dev,"%s","/dev/video2");
    sprintf(isp_dev,"%s","/dev/video1");
    ret = v4l2_init(v4l2_dev,isp_dev);
    if (ret<0){
        printf("V4l2 init fail\n");
        return 0;
    }
    rga_init();
    disp_init();

    printf("Enter main loop\n");
    while(1){
        process(tDemo.image.data);
        printf("FRM:%4d CAPQ:%3d %3d RGA:%3d %3d CAPDQ:%3d %3d DISP:%3d %3d ALL:%3d %3d\n",tDemo.count,
            tDemo.time[0],tDemo.timeall[0]/tDemo.count,
            tDemo.time[1],tDemo.timeall[1]/tDemo.count,
            tDemo.time[2],tDemo.timeall[2]/tDemo.count,
            tDemo.time[3],tDemo.timeall[3]/tDemo.count,
            tDemo.time[4],tDemo.timeall[4]/tDemo.count);
    }
    rga_uninit();
    v4l2_uninit();
}
