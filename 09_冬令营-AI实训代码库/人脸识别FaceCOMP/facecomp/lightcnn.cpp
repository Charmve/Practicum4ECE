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
 *
 */
#include "lightcnn.hpp"
#include <iostream>

lightcnn::lightcnn()
{
    faceSize = FACEWIDTH * FACEHEIGHT;

}

/* load model and create graph */
int lightcnn::init(const std::string& model_dir)
{
    //std::string proto_name_ = model_dir+"/LightenedCNN_B.prototxt";
    //std::string mdl_name_ = model_dir+"/LightenedCNN_B.caffemodel";
    std::string mdl_name = model_dir+"/LightenedCNN_B.tmfile";
    //if (load_model(LCNN_model_name, "caffe", proto_name_.c_str(), mdl_name_.c_str()) < 0)
    //{
      //  std::cout << "load model failed\n";
       // return -1;

    //}

    LCNN_graph = create_graph(nullptr, "tengine",mdl_name.c_str());

    if (LCNN_graph == nullptr)
    {
        std::cout << "create graph failed\n";
	std::cout << "errno: "<<get_tengine_errno() << "\n";
	return 1;
    }


    return 0;
}

int lightcnn::featureExtract(fcv::Mat& img,float *feature)
{


    fcv::Mat imgGray(FACEWIDTH,FACEHEIGHT,CV_8UC1);
    fcv::Mat imgFace(FACEWIDTH,FACEHEIGHT,CV_32FC1);
    float alpha=0.00390625;
    int dims[] = {1, 1, FACEHEIGHT, FACEWIDTH};
    static bool first_run = true;
    /* convert BGR image to Gray image */
    fcv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    /* convert int8 image to fp32 image */
    imgGray.convertTo(imgFace, CV_32FC1);

    /* normalize to [0,1) */
    imgFace=imgFace*alpha;

    /* get a tensor handle of lcnn graph input */
    input_tensor = get_graph_tensor(LCNN_graph, "data");

    /* set the shape of lcnn tensor */
    set_tensor_shape(input_tensor, dims, 4);

    /* set the buffer of the tensor */
    if (set_tensor_buffer(input_tensor, imgFace.data, faceSize * 4) < 0)
    {
        std::cout << "set buffer for input tensor failed\n";
        return -1;
    }

    /* initialize resource for graph execution */
    //prerun_graph(LCNN_graph);

    /* execute graph */
    //run_graph(LCNN_graph, 1);
    if(first_run)
    {
        if(prerun_graph(LCNN_graph) != 0)
            {
	           std::cout << "Prerun PNet graph failed, errno: " << get_tengine_errno() << "\n";
                   return 1;
            }
        first_run = false;
    }

    if(run_graph(LCNN_graph, 1) != 0)
        {
            std::cout << "Run PNet graph failed, errno: " << get_tengine_errno() << "\n";
            return 1;
        }


    /* convert output tensor name into a tensor handle */
    tensor_t out_tensor = get_graph_tensor(LCNN_graph, "eltwise_fc1");

    /* get the buffer of output tensor */
    float *data = (float *)get_tensor_buffer(out_tensor);
    memcpy(feature,data, FEATURESIZE * sizeof(float));

    /* release the input and output tensor handle */
    put_graph_tensor(out_tensor);
    put_graph_tensor(input_tensor);

    /* release resource for graph execution */
    postrun_graph(LCNN_graph);


    return 0;
}

/* release tengine resources */
lightcnn::~lightcnn()
{
    destroy_runtime_graph(LCNN_graph);
    remove_model(LCNN_model_name);
}



