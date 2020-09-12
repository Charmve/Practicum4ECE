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

#ifndef __MIPI_CAM_HPP__
#define __MIPI_CAM_HPP__

#include <functional>
#include <iostream>
#include <pthread.h>
#include <sys/syscall.h>
#include <getopt.h>
#include <chrono>
#include <asm/types.h>
#include <assert.h>
#include <fcntl.h>
#include <dirent.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <linux/stddef.h>
#include <linux/videodev2.h>
#include <errno.h>

#include "openai_io.hpp"
#include "fastcv.hpp"
extern "C" {
#include <rockchip/rockchip_rga.h>
}
#include <rockchip/rockchip_isp.h>

using namespace fcv;
using namespace std;

enum CAM_TYPE {
    CAM_OV9750 = 0,
    CAM_IMX258,
    CAM_MAX,
};

struct buffer {
    void *start;
    size_t length;
};


class v4l2Camera {

private:
    pthread_mutex_t mutex;
    RockchipRga *rga;
    int fd;

    struct buffer *buffers = NULL;
    int n_buffers;

    __u32 width;
    __u32 height;
    RgaRotate rotate;
    __u32 cropx;
    __u32 cropy;
    __u32 cropw;
    __u32 croph;
    __u32 format;

    int recv_count;

    void* rkengine;
    enum CAM_TYPE cam_type;

public:
    v4l2Camera(__u32 w, __u32 h, RgaRotate r, int V, int H, __u32 cx, __u32 cy, __u32 cw, __u32 ch, __u32 cam_format);
    ~v4l2Camera();


    unsigned long microTime();
    int init(const char *v4l2_dev, char *isp_dev, enum CAM_TYPE type);
    int streamOn(void);
    int streamOff(void);
    int readFrame(__u32 dstFormat, Mat& image);
    void rgaConvertFormat(Mat src, Mat dst, __u32 srcFormat, __u32 dstFormat, __u32 width, __u32 height);
    RgaBuffer *allocBuffer(__u32 v4l2Format, __u32 width, __u32 height);
    void freeBuffer(RgaBuffer *buf);
};


#endif
