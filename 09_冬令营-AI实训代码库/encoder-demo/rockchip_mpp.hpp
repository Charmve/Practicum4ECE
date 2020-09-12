#include <functional>
#include <iostream>
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

#include <rockchip/rockchip_mpp.h>

class mpp_decoder {
public:
	mpp_decoder(void);
	~mpp_decoder(void);
	int init(DecodeType type);
	int enqueue(void *buf, size_t len);
	DecFrame *dequeue(void);
	void freeFrame(DecFrame *frame);
private:
	MppDecoder *dec;
};


class mpp_encoder {
public:
	mpp_encoder(__u32 format, __u32 width, __u32 height, size_t size);
	mpp_encoder(__u32 format, __u32 width, __u32 height, size_t size,
                    int fps, int gop, EncodeRcMode mode, EncodeQuality quality);
	~mpp_encoder(void);
	int init(void);
	/* import fds, which are allocated by rga interface */
	int importFds(int *fd, int count);
	/* Get extra data(sps and pps) */ 
	EncPacket *extraData(void);
	int enqueue(int index);
	EncPacket *dequeue(void);
	void freePacket(EncPacket *packet);
private:
	EncCtx ctx;
	MppEncoder *enc;
};
