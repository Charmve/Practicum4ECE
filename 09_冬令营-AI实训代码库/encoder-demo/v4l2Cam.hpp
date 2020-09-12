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
#include <time.h>
#include <unistd.h>

#include <linux/stddef.h>
#include <linux/videodev2.h>
#include <errno.h>

#include <rockchip/rockchip_isp.h>

typedef enum __camera_type {
	CAMERA_TYPE_NA,
	CAMERA_TYPE_USB,
	CAMERA_TYPE_OV9750,
	CAMERA_TYPE_IMX258,
} CameraType;

struct camera_buffer {
	int fd;
	void *start;
	size_t length;
};

class v4l2Cam {
public:
	v4l2Cam(void);
	~v4l2Cam(void);
	int init(const char *dev, CameraType t);
	int setfmt(__u32 w, __u32 h, __u32 f);
	int reqbufs_userptr(__u32 count, void **ptrs, size_t len);
	int reqbufs_dma(__u32 count, int *fds, size_t len);
	int reqbufs_mmap(__u32 count);
	int start(void);
	int enqueue(int index);
	void *dequeue(int *index);

private:
	int fd;
	__u32 width;
	__u32 height;
	__u32 format;

	CameraType type;
	void* rkengine;

	struct camera_buffer *buffers;
	int n_buffers;
	__u32 memory;
};
