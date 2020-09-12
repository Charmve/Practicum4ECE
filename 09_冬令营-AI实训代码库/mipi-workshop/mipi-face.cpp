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

/* Algorithm -- add header files here */

#include "mipi_cam.hpp"
#include "mtcnn_utils.hpp"
#include "mtcnn.hpp"
#include <string>
#include <sys/time.h>

/* Algorithm -- add color definitions */
#define COLOR_GREEN fcv::Scalar(0, 255, 0)
#define COLOR_RED   fcv::Scalar(0, 0, 255)
#define COLOR_BLUE  fcv::Scalar(255, 0, 0)
#define COLOR_YELLOW  fcv::Scalar(0, 255, 255)
#define COLOR_MAGENTA fcv::Scalar(255, 0, 255)
#define COLOR_CYAN    fcv::Scalar(255, 255, 0)


const string wintitle = "mipi-camera";


static void usage(char **argv)
{
    printf(
        "Usage: %s [Options]\n\n"
        "Options:\n"
        "-m, --mipi                   Mipi camera: 1 -- mipi1 cmaera; 2 -- mipi2 camera, default: 1"
        "-t, --type                   Mipi camera type: ov9750 or imx258, default: ov9750"
        "-w, --width                  Destination images's width\n"
        "-h, --height                 Destination images's height\n"
        "-r, --rotate                 Image rotation degree, the value should be 90, 180 or 270\n"
        "-V, --vflip                  Vertical Mirror\n"
        "-H, --hflip                  Horizontal Mirror\n"
        "-c, --crop                   Crop, format: x,y,w,h\n"
        "\n",
        argv[0]);
}

static const char *short_options = "m:t:w:h:r:VHc:";

static struct option long_options[] = {
    {"mipi", required_argument, NULL, 'm'},
    {"type", required_argument, NULL, 't'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"rotate", required_argument, NULL, 'r'},
    {"vflip", no_argument, NULL, 'V'},
    {"hflip", no_argument, NULL, 'H'},
    {"crop", required_argument, NULL, 'c'},
    {NULL, 0, NULL, 0}
};

static void parse_crop_parameters(char *crop, __u32 *cropx, __u32 *cropy, __u32 *cropw, __u32 *croph)
{
    char *p, *buf;
    const char *delims = ".,";
    __u32 v[4] = {0,0,0,0};
    int i = 0;

    buf = strdup(crop);
    p = strtok(buf, delims);
    while(p != NULL) {
        v[i++] = atoi(p);
        p = strtok(NULL, delims);

        if(i >=4)
            break;
    }

    *cropx = v[0];
    *cropy = v[1];
    *cropw = v[2];
    *croph = v[3];
}

int main(int argc, char **argv)
{
    //std::string img_name = "./images/mtcnn_face4.jpg";
    //std::string sv_name = "result.jpg";

    int ret, c, r;
    char v4l2_dev[64], isp_dev[64];
    char index = -1;
    pthread_t id;
    Mat image;
    struct timeval t0, t1;
	
	cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0,&mask);

    /* Algorithm -- variable definition */
	std::string model_dir = "./models/";
    std::vector<face_box> face_info;
	
    /* Tengine -- initialization */
    if (init_tengine_library()<0)
    {
	std::cout << " init tengine failed\n";
	return 1;
    }
    if(request_tengine_version("0.9")<0)
        return -1;

    if(argc==1)
    {
        std::cout<<"[usage]: "<<argv[0]<<" <test.jpg>  <model_dir> [save_result.jpg] \n";
    }

    std::cout << "Tengine Version: " << get_tengine_version() << endl;
    //if(argc >=2 ) img_name=argv[1];
    if(argc >=3 ) model_dir=argv[2];
    //if(argc >=4 ) sv_name=argv[3];

    /* Algorithm -- default value */
    int min_size=60;
    float conf_p=0.6;
    float conf_r=0.7;
    float conf_o=0.8;
    float nms_p=0.5;
    float nms_r=0.7;
    float nms_o=0.7;
    /* Algorithm -- initialization */
    mtcnn* det = new mtcnn(min_size,conf_p,conf_r,conf_o,nms_p,nms_r,nms_o);
    /* Algorithm -- load models */
	det->load_3model(model_dir);
    /* Window -- create */
    fcv::namedWindow(wintitle);
    fcv::moveWindow(wintitle, 720, 480);

    /* MIPI Camera -- default values */
    int mipi = 1;    /* main camera */
    enum CAM_TYPE type = CAM_OV9750; /* HD camera sensor: OV9750 */
    __u32 width = 640, height = 480; /* resolution: 640x480 */
    RgaRotate rotate = RGA_ROTATE_NONE; /* No rotation */
    __u32 cropx = 0, cropy = 0, cropw = 0, croph = 0;
    int vflip = 0, hflip = 0; /* no flip */

    while((c = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (c) {
        case 'm':
            mipi = atoi(optarg);
            break;
        case 't':
            if(strncmp(optarg, "ov9750", 6) == 0)
                type = CAM_OV9750;
            if(strncmp(optarg, "imx258", 6) == 0)
                type = CAM_IMX258;
            break;
        case 'w':
            width = atoi(optarg);
            break;
        case 'h':
            height = atoi(optarg);
            break;
        case 'r':
            r = atoi(optarg);
            switch(r) {
            case 0:
                rotate = RGA_ROTATE_NONE;
                break;
            case 90:
                rotate = RGA_ROTATE_90;
                break;
            case 180:
                rotate = RGA_ROTATE_180;
                break;
            case 270:
                rotate = RGA_ROTATE_270;
                break;
            default:
                printf("roate %d is not supported\n", r);
                return -1;
            }
            break;
        case 'V':
            vflip = 1;
            break;
        case 'H':
            hflip = 1;
            break;
        case 'c':
            parse_crop_parameters(optarg, &cropx, &cropy, &cropw, &croph);
            break;
        default:
            usage(argv);
            return 0;
        }
    }

    if (type == CAM_IMX258)
    {
        type = CAM_OV9750;
        printf ("IMX258 is not supported currently. Use OV9750 instead!\n");
    }

    /* V4L2 device */
    sprintf(v4l2_dev, "/dev/video%d", 4 * (mipi - 1) + 2);
    sprintf(isp_dev, "/dev/video%d", 4 * (mipi - 1) + 1);

    printf("width = %u, height = %u, rotate = %u, vflip = %d, hflip = %d, crop = [%u, %u, %u, %u]\n",
           width, height, rotate, vflip, hflip, cropx, cropy, cropw, croph);



    /* MIPI Camera -- initialization */
    v4l2Camera v4l2(width, height, rotate, vflip, hflip, cropx, cropy, cropw, croph, V4L2_PIX_FMT_NV12);
    image.create(cv::Size(RGA_ALIGN(width, 16), RGA_ALIGN(height, 16)), CV_8UC3);
    ret = v4l2.init(v4l2_dev, isp_dev, type);
    if(ret < 0)
    {
        printf("v4l2Camera initialization failed.\n");
        return ret;
    }

    /* MIPI Camera -- open stream */
    ret = v4l2.streamOn();
    if(ret < 0)
        return ret;
    while(1) {
        gettimeofday(&t0, NULL);
        /* MIPI Camera -- read video frame */
        if(ret = v4l2.readFrame(V4L2_PIX_FMT_RGB24,image) < 0)
            return ret;

        /* Algorithm -- detect faces in image */
	face_info.clear();
        det->detect(image, face_info);
        /* Algorithm -- drawing results	*/
	for (unsigned int i = 0; i < face_info.size(); i++)
	    {
	        face_box &box = face_info[i];
	        std::printf("Face box(%d):( %g , %g ),( %g , %g )\n", i, box.x0, box.y0, box.x1, box.y1);
	        fcv::rectangle(image, fcv::Point(box.x0, box.y0), fcv::Point(box.x1, box.y1), COLOR_GREEN, 1);
	        for (int l = 0; l < 5; l++)
	        {
	            fcv::circle(image, fcv::Point(box.landmark.x[l], box.landmark.y[l]), 1, COLOR_MAGENTA, 2);
	        }
    	   }
        /* Window -- drawing frame */
        fcv::imshow(wintitle, image, NULL);
        gettimeofday(&t1, NULL);
        fcv::waitKey(1);
        long elapse = ((t1.tv_sec * 1000000 + t1.tv_usec) - (t0.tv_sec * 1000000 + t0.tv_usec)) / 1000;

        cout <<"fastcv::imshow costs "<< elapse <<" miliseconds"<< endl;
    }

    /* MIPI Camera -- close stream */
    ret = v4l2.streamOff();

    /* Tengine -- deinitialization */
	release_tengine_library();

    if(ret < 0)
        return ret;
    return 0;
}

