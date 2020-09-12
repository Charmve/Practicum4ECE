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
 * Author: minglu@openailab.com
 *
 */

#include "mipi_cam.hpp"


v4l2Camera::v4l2Camera(__u32 w, __u32 h, RgaRotate r, int V, int H, __u32 cx, __u32 cy, __u32 cw, __u32 ch, __u32 cam_format)
{
    rga = NULL;
    fd = -1;
    recv_count = 0;

    rkengine = NULL;
    cam_type = CAM_MAX;

    width = w;
    height = h;
    if(V == 1)
        rotate = RGA_ROTATE_VFLIP;
    else if(H == 1)
        rotate = RGA_ROTATE_HFLIP;
    else
        rotate = r;
    cropx = cx;
    cropy = cy;
    cropw = cw;
    croph = ch;
    format = cam_format;
}

v4l2Camera::~v4l2Camera()
{
    printf("Quit!\n");

    /* Call rockchip_isp interface */
    if(cam_type == CAM_IMX258)
        rkisp_setFocusMode(rkengine, HAL_AF_MODE_NOT_SET);
    rkisp_stop(rkengine);

    if(rga != NULL)
        RgaDestroy(rga);
    for(int i = 0; i < n_buffers; i++) {
        munmap(buffers[i].start, buffers[i].length);
    }
    free(buffers);
    close(fd);
}

unsigned long v4l2Camera::microTime()
{
    struct timeval t;

    gettimeofday(&t, NULL);

    return (unsigned long)(t.tv_sec) * 1000000 + t.tv_usec;
}

int v4l2Camera::init(const char *v4l2_dev, char *isp_dev, enum CAM_TYPE type)
{
    int ret = 0, found = 0;
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_fmtdesc fmtdesc;
    struct v4l2_requestbuffers req;

    cam_type = type;

    fd = open(v4l2_dev, O_RDWR, 0);
    if(fd < 0) {
        printf("Open dev %s failed\n", v4l2_dev);
        return fd;
    }

    ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
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

    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(1) {
        ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
        if(ret < 0)
            break;

        if(fmtdesc.pixelformat == format) {
            found = 1;
            break;
        }
        fmtdesc.index++;
    }

    if(found == 0) {
        printf("This format is NOT supported\n");
        return -1;
    }

    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.pixelformat = format;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
    if(ret < 0) {
        printf("Set format failed\n");
        return ret;
    }

    memset(&req, 0, sizeof(req));
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(fd, VIDIOC_REQBUFS, &req);
    if(ret < 0) {
        printf("Request buffers failed\n");
        return ret;
    }

    buffers = (struct buffer *)calloc(req.count, sizeof(struct buffer));
    n_buffers = req.count;
    if(!buffers) {
        printf("Out of memory\n");
        return -1;
    }

    for(int i = 0; i < req.count; i++) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
        if(ret < 0) {
            printf("Query buffers failed\n");
            return ret;
        }
        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
        if(MAP_FAILED == buffers[i].start) {
            printf("mmap buffers failed\n");
            return ret;
        }
    }

    rga = RgaCreate();
    if(rga == NULL) {
        return -1;
    }

    /* Call rockchip_isp interface */
    if(cam_type == CAM_OV9750) {
        rkisp_start(rkengine, fd, isp_dev, "/etc/cam_iq_ov9750.xml");
    } else if(cam_type == CAM_IMX258) {
        rkisp_start(rkengine, fd, isp_dev, "/etc/cam_iq_imx258.xml");
        rkisp_setFocusMode(rkengine, HAL_AF_MODE_CONTINUOUS_VIDEO);
    }

    return 0;
}

int v4l2Camera::streamOn(void)
{
    int ret;
    enum v4l2_buf_type type;

    for(int i = 0; i < n_buffers; i++) {
        struct v4l2_buffer buf;

        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        ret = ioctl(fd, VIDIOC_QBUF, &buf);
        if(ret < 0) {
            printf("Queue buffers failed\n");
            return ret;
        }
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if(ret < 0) {
        printf("Stream on failed\n");
        return ret;
    }

    return 0;
}

int v4l2Camera::streamOff(void)
{
    int ret;
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if(ret < 0) {
        printf("Stream off failed\n");
        return ret;
    }

    return 0;
}

int v4l2Camera::readFrame(__u32 dstFormat, Mat& image)
{
    int ret;
    unsigned long t1, t2;
    struct v4l2_buffer buf;

    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(fd, VIDIOC_DQBUF, &buf);
    if(ret < 0) {
        printf("Dequeue buffer failed, ret = %d\n", ret);
        return ret;
    }

    t1 = microTime();
    rga->ops->initCtx(rga);
    rga->ops->setSrcBufferPtr(rga, buffers[buf.index].start);
    rga->ops->setDstBufferPtr(rga, image.data);

    rga->ops->setSrcFormat(rga, format, width, height);
    rga->ops->setDstFormat(rga, dstFormat, width, height);

    rga->ops->setRotate(rga, rotate);
    if(cropw > 0 && croph > 0) {
        rga->ops->setSrcCrop(rga, cropx, cropy, cropw, croph);
        rga->ops->setDstCrop(rga, 0, 0, width, height);
    }

    rga->ops->go(rga);
    t2 = microTime();
    printf("Rga process done(%dx%d), time %llums\n",width, height, (t2 - t1) /1000);

    ret = ioctl(fd, VIDIOC_QBUF, &buf);
    if(ret < 0) {
        printf("Eequeue buffer failed\n");
        return ret;
    }
    return 0;
}


void v4l2Camera::rgaConvertFormat(Mat src, Mat dst, __u32 srcFormat, __u32 dstFormat, __u32 width, __u32 height)
{
    rga->ops->initCtx(rga);
    rga->ops->setSrcBufferPtr(rga, src.data);
    rga->ops->setDstBufferPtr(rga, dst.data);

    rga->ops->setSrcFormat(rga, srcFormat, width, height);
    rga->ops->setDstFormat(rga, dstFormat, width, height);

    rga->ops->go(rga);
}

RgaBuffer *v4l2Camera::allocBuffer(__u32 v4l2Format, __u32 width, __u32 height)
{
    return rga->ops->allocDmaBuffer(rga, v4l2Format, width, height);
}

void v4l2Camera::freeBuffer(RgaBuffer *buf)
{
    rga->ops->freeDmaBuffer(rga, buf);
}






