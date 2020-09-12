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
 * Author: chunyinglv@openailab.com
 */
#include <unistd.h>

#include <iostream> 
#include <functional>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <time.h>


#include "tengine_c_api.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "common_util.hpp"

#define PRINT_TOP_NUM            1

const float channel_mean[3]={127.5,127.5,127.5};

using namespace TEngine;


void get_input_data(const char *image_file, float *input_data,
        int img_h, int img_w, const float* mean, float scale)
{
    cv::Mat sample = cv::imread(image_file, -1);
    if (sample.empty())
    {
        std::cerr << "Failed to read image file " << image_file << ".\n";
        return;
    }
    cv::Mat img;
    if (sample.channels() == 4) 
    {
        cv::cvtColor(sample, img, cv::COLOR_BGRA2BGR);
    }
    else if (sample.channels() == 1)
    {
        cv::cvtColor(sample, img, cv::COLOR_GRAY2BGR);
    }
    else
    {
        img=sample;
    }
    
    cv::resize(img, img, cv::Size(img_h, img_w));
    img.convertTo(img, CV_32FC3);
    float *img_data = (float *)img.data;
    int hw = img_h * img_w;
    for (int h = 0; h < img_h; h++)
    {
        for (int w = 0; w < img_w; w++)
        {
            for (int c = 0; c < 3; c++)
            {
                input_data[c * hw + h * img_w + w] = (*img_data - mean[c])*scale;
                img_data++;
            }
        }
    }
}

static inline std::vector<int> Argmax1(const std::vector<float> &v, int N)
{
    std::vector<std::pair<float, int>> pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], i));
    std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

    std::vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}
   
int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        std::cout << "[Usage]: " << argv[0] << " <modelfile> <img>\n";
        return 0;
    }

    // init tengine
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
    
    std::string mdl_name = argv[1];
    std::string image_file = argv[2];

	fcv::Mat image = fcv::imread(img_name);
    if (image.empty())
    {
        std::cerr<<"fcv::imread "<<img_name<<" failed\n";
        return -1;
    }

    // load model and create_graph
    mdl_name= model_dir+"/det1.tmfile";
    if(!check_file_exist(mdl_name))
	       return 1;
                
    graph=create_graph(nullptr, "tengine", mdl_name.c_str()); 
    if(graph == nullptr)
    {
        std::cout << "Create Graph failed\n";
        std::cout << "errno: " << get_tengine_errno() << "\n";
            return 1;
    }

    int img_h=28;
    int img_w=28;

    //define inputdata and malloc memory and get input data
    

    const char *input_tensor_name = "data";
    tensor_t input_tensor = get_graph_tensor(graph, input_tensor_name);
    int dims[] = {1, 3, img_h, img_w};

    //set  tensor shape 
    set_tensor_shape(input_tensor, dims, 4);

    //preprun graph
	if(prerun_graph(graph) < 0) 
     { 
         std::cerr << "Prerun graph failed\n"; 
         return false; 
     } 
 

     struct timeval t0, t1; 
     float avg_time = 0.f; 
     for(int i = 0; i < repeat_count; i++) 
     { 
         get_input_data(image_file, input_data, img_h, img_w, mean, scale); 
         set_tensor_buffer(input_tensor, input_data, img_size * 4); 
 

         gettimeofday(&t0, NULL); 
         if(run_graph(graph, 1) < 0) 
         { 
             std::cerr << "Run graph failed\n"; 
             return false; 
         } 
         gettimeofday(&t1, NULL); 
 

         float mytime = ( float )((t1.tv_sec * 1000000 + t1.tv_usec) - (t0.tv_sec * 1000000 + t0.tv_usec)) / 1000; 
         avg_time += mytime; 
     } 
	 std::cout << "\nModel name : " << model_name << "\n" 
               << "Proto file : " << proto_file << "\n" 
               << "Model file : " << model_file << "\n" 
               << "label file : " << label_file << "\n" 
               << "image file : " << image_file << "\n" 
               << "img_h, imag_w, scale, mean[3] : " << img_h << " " << img_w << " " << scale << " " << mean[0] << " " 
               << mean[1] << " " << mean[2] << "\n"; 
     std::cout << "\nRepeat " << repeat_count << " times, avg time per run is " << avg_time / repeat_count << " ms\n"; 
     std::cout << "--------------------------------------\n"; 
    
    //set tensor buffer

    //run graph

    //define and init output tensor 
    tensor_t output_tensor = get_graph_output_tensor(graph, 0, 0);

    float *data = (float *)get_tensor_buffer(output_tensor);
    float *end = data + 10;
    std::vector<float> result(data, end);
    std::vector<int> top_N = Argmax1(result, PRINT_TOP_NUM);

    for (unsigned int i = 0; i < top_N.size(); i++)
    {
        int idx = top_N[i];

	    std::cout<<"Predict Result:" <<idx << "\n";
    }
    free(input_data);

    //postrun graph
	postrun_graph(graph);
    //destroy runtime graph
	destroy_graph(graph);
    //remove model
	delete det;
    //release tengine library
	release_tengine();
    return 0;
}
