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
#ifndef __LIGHTCNN_HPP__
#define __LIGHTCNN_HPP__

#include<string>
#include "fastcv.hpp"
#include "openai_io.hpp"
#include "tengine_c_api.h"

#define FEATURESIZE 256
#define FACEWIDTH 128
#define FACEHEIGHT 128



class lightcnn {
public:


    lightcnn();
    ~lightcnn();


    int init(const std::string& model_dir);

    int featureExtract(fcv::Mat& img,float *feature);


private:
    const char * LCNN_model_name="LightCNN";
    graph_t LCNN_graph;
    int faceSize;
    tensor_t input_tensor;
    tensor_t out_tensor;
};



#endif