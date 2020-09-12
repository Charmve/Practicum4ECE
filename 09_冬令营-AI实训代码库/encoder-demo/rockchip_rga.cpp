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

#include <rockchip_rga.hpp>

rockchip_rga::rockchip_rga(void)
{
	rga = NULL;
}


rockchip_rga::~rockchip_rga(void)
{
	if(rga)
		RgaDestroy(rga);
}

int rockchip_rga::init(void)
{
	rga = RgaCreate();
	if(rga == NULL) {
		fprintf(stderr, "Create rga failed\n");
		return -EIO;
	}

	return 0;
}

RgaBuffer *rockchip_rga::alloc(__u32 format, __u32 width, __u32 height)
{
	return rga->ops->allocDmaBuffer(rga, format, width, height);
}

void rockchip_rga::free(RgaBuffer *buf)
{
	rga->ops->freeDmaBuffer(rga, buf);
}

int rockchip_rga::convert(void *src_buf, void *dst_buf, struct rga_format src_fmt,
		      struct rga_format dst_fmt, struct rga_crop crop, RgaRotate rotate)
{
	rga->ops->initCtx(rga);
	rga->ops->setSrcBufferPtr(rga, src_buf);
	rga->ops->setDstBufferPtr(rga, dst_buf);

	rga->ops->setSrcFormat(rga, src_fmt.format, src_fmt.width, src_fmt.height);
	rga->ops->setDstFormat(rga, dst_fmt.format, dst_fmt.width, dst_fmt.height);

	rga->ops->setRotate(rga, rotate);
	if(crop.w > 0 && crop.h > 0) {
		rga->ops->setSrcCrop(rga, crop.x, crop.y, crop.w, crop.h);
		rga->ops->setDstCrop(rga, 0, 0, dst_fmt.width, dst_fmt.height);
	}

	return rga->ops->go(rga);
}
