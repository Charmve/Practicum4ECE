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

v4l2Cam::v4l2Cam()
{
	fd = -1;
	width = 0;
	height = 0;
	format = 0;
	buffers = NULL;
	type = CAMERA_TYPE_USB;
}

v4l2Cam::~v4l2Cam()
{
	int i;
	enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(fd, VIDIOC_STREAMOFF, &buf_type);

	switch(type) {
	case CAMERA_TYPE_USB:
		break;
	case CAMERA_TYPE_IMX258:
		if(rkengine)
			rkisp_setFocusMode(rkengine, HAL_AF_MODE_NOT_SET);
	case CAMERA_TYPE_OV9750:
		if(rkengine)
			rkisp_stop(rkengine);
		break;
	default:
		break;
	}

	if(buffers != NULL) {
		if(memory == V4L2_MEMORY_MMAP) {
			for(i = 0; i < n_buffers; i++){
				munmap(buffers[i].start, buffers[i].length);
			}
		}
		free(buffers);
		buffers = NULL;
	}

	if(fd >= 0) {
		close(fd);
		fd = -1;
	}
}

int v4l2Cam::init(const char *dev, CameraType t)
{
	int ret = 0, index;
	struct v4l2_capability cap;
	char v4l2_dev[32], isp_dev[32];
	printf("CameraType(%d)\n",t);
	sscanf(dev, "/dev/video%d", &index);
	sprintf(v4l2_dev, "%s", dev);
	if(t != CAMERA_TYPE_USB)
		sprintf(isp_dev, "/dev/video%d", index - 1);
	
	printf("v4l2Dev(%s) ispDev(%s)\n",v4l2_dev,isp_dev);

	fd = open(v4l2_dev, O_RDWR, 0);
	if(fd < 0) {
		fprintf(stderr, "Open dev %s failed\n", v4l2_dev);
		return fd;
	}

	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if(ret < 0) {
		fprintf(stderr, "Querycap failed\n");
		goto exit;
	}

	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s is NOT video capture device\n", v4l2_dev);
		ret = -ENOSTR;
		goto exit;
	}

	if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
		fprintf(stderr, "%s is NOT video streaming io\n", v4l2_dev);
		ret = -ENOSTR;
		goto exit;
	}

	switch(t) {
	case CAMERA_TYPE_USB:
		break;
	case CAMERA_TYPE_OV9750:
		rkisp_start(rkengine, fd, isp_dev, "/etc/cam_iq_ov9750.xml");
		break;
	case CAMERA_TYPE_IMX258:
		rkisp_start(rkengine, fd, isp_dev, "/etc/cam_iq_imx258.xml");
		rkisp_setFocusMode(rkengine, HAL_AF_MODE_CONTINUOUS_VIDEO);
		break;
	default:
		fprintf(stderr, "Camera type(%d) is NOT supported\n", t);
		ret = -EIO;
		goto exit;

	}

	type = t;
	return 0;
exit:
	close(fd);
	return ret;
}

int v4l2Cam::setfmt(__u32 w, __u32 h, __u32 f)
{
	int ret, found = 0;
	struct v4l2_format fmt;
	struct v4l2_fmtdesc fmtdesc;

	fmtdesc.index = 0;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	while(1) {
		ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
		if(ret < 0)
			break;

		if(fmtdesc.pixelformat == f) {
			found = 1;
			break;
		}
		fmtdesc.index++;
	}

	if(found == 0) {
		fprintf(stderr, "This format(%u) is NOT supported\n", f);
		return -EINVAL;
	}

	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = w;
	fmt.fmt.pix.height = h;
	fmt.fmt.pix.pixelformat = f;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
	if(ret < 0) {
		fprintf(stderr, "Set format failed\n");
		return ret;
	}

	width = w;
	height =h;
	format = f;
	return 0;
}

int v4l2Cam::reqbufs_mmap(__u32 count)
{
	int ret = 0;
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count = count;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if(ret < 0) {
		fprintf(stderr, "Request buffers failed\n");
		return ret;
	}

	buffers = (struct camera_buffer *)calloc(req.count, sizeof(struct camera_buffer));
	n_buffers = req.count;
	if(!buffers) {
		fprintf(stderr, "Request buffers: Out of memory\n");
		return -ENOMEM;
	}

	for(__u32 i = 0; i < req.count; i++) {
		struct v4l2_buffer buf;

		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
		if(ret < 0) {
			fprintf(stderr, "Query buffers failed\n");
			return ret;
		}

		buffers[i].length = buf.length;
		buffers[i].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
		if(MAP_FAILED == buffers[i].start) {
			fprintf(stderr, "mmap buffers failed\n");
			return ret;
		}
	}


	memory = V4L2_MEMORY_MMAP;
	return 0;
}

int v4l2Cam::reqbufs_userptr(__u32 count, void **ptrs, size_t len)
{
	int ret;
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count = count;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_USERPTR;
	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if(ret < 0) {
		fprintf(stderr, "Request buffers failed\n");
		return ret;
	}

	buffers = (struct camera_buffer *)calloc(req.count, sizeof(struct camera_buffer));
	n_buffers = req.count;
	if(!buffers) {
		fprintf(stderr, "Request buffers: Out of memory\n");
		return -ENOMEM;
	}

	for(__u32 i = 0; i < req.count; i++) {
		buffers[i].start = ptrs[i];
		buffers[i].length = len;
	}

	memory = V4L2_MEMORY_USERPTR;
	return 0;
}

int v4l2Cam::reqbufs_dma(__u32 count, int *fds, size_t len)
{
	int ret;
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count = count;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_DMABUF;
	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if(ret < 0) {
		fprintf(stderr, "Request buffers failed\n");
		return ret;
	}

	buffers = (struct camera_buffer *)calloc(req.count, sizeof(struct camera_buffer));
	n_buffers = req.count;
	if(!buffers) {
		fprintf(stderr, "Request buffers: Out of memory\n");
		return -ENOMEM;
	}

	for(__u32 i = 0; i < req.count; i++) {
		buffers[i].fd = fds[i];
		buffers[i].length = len;
	}

	memory = V4L2_MEMORY_DMABUF;
	return 0;
}

int v4l2Cam::enqueue(int index)
{
	int ret;
	struct v4l2_buffer buf;

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = memory;
	buf.index = index;
	if(memory == V4L2_MEMORY_USERPTR) {
		buf.m.userptr = (unsigned long)buffers[index].start;
		buf.length = buffers[index].length;
	}else if(memory == V4L2_MEMORY_DMABUF) {
		buf.m.fd = (unsigned long)buffers[index].fd;
		buf.length = buffers[index].length;
	}

	ret = ioctl(fd, VIDIOC_QBUF, &buf);
	if(ret < 0) {
		fprintf(stderr, "Enqueue buffers failed\n");
		return ret;
	}

	return 0;
}

void *v4l2Cam::dequeue(int *index)
{
	int ret;
	struct v4l2_buffer buf;

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = memory;

	ret = ioctl(fd, VIDIOC_DQBUF, &buf);
	if(ret < 0) {
		fprintf(stderr, "Dequeue buffer failed, ret = %d\n", ret);
		return NULL;
	}

	*index = buf.index;
	return buffers[buf.index].start;
}

int v4l2Cam::start()
{
	int ret;
	enum v4l2_buf_type buf_type;

	for(int i = 0; i < n_buffers; i++) {
		ret = enqueue(i);
		if(ret < 0)
			return ret;
	}

	buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_STREAMON, &buf_type);
	if(ret < 0) {
		fprintf(stderr, "Stream on failed\n");
		return ret;
	}

	return 0;
}
