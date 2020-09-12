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
 * Author: minglu@openailab.comm
 *
 */
#include <string>
#include <sys/time.h>
#include "mtcnn.hpp"
#include "lightcnn.hpp"
#include "mtcnn_utils.hpp"


/* calculate cosine distance of two vectors */
float cosine_dist(float* vectorA, float* vectorB, int size)
{
    float Numerator=0;
    float Denominator1=0;
    float Denominator2=0;
    float Similarity;
    for (int i = 0 ; i < size ; i++)
    {
        Numerator += (vectorA[i] * vectorB[i]);
        Denominator1 += (vectorA[i] * vectorA[i]);
        Denominator2 += (vectorB[i] * vectorB[i]);
    }

    Similarity = Numerator/sqrt(Denominator1)/sqrt(Denominator2);

    return Similarity;
}

/* get face image with face detection results */
void get_face_image(fcv::Mat& img,std::vector<face_box>& face_list, fcv::Mat& faceImg)
{
    float boxSize=0;
    float maxWidth=0;
    int maxI = 0;
    float faceWidth = 0;
    fcv::Size faceSize(FACEWIDTH,FACEHEIGHT);


    /* Select face with largest size */
    for ( unsigned int i = 0; i < face_list.size(); i++)
    {
        face_box &box = face_list[i];
        boxSize = box.x1 - box.x0;
        if (boxSize>maxWidth)
        {
            maxWidth = boxSize;
            maxI = i;
        }
    }
    face_box &box = face_list[maxI];

    /* calculate face width in oringal image */
    faceWidth = (box.landmark.x[1] - box.landmark.x[0])/0.53194925;

    /* calculate face roi for image cropping */
    fcv::Rect roi;
    roi.x = box.landmark.x[0]-(0.224152*faceWidth);
    roi.y = (box.landmark.y[0]+box.landmark.y[1])/2-(0.2119465*faceWidth);
    roi.width = faceWidth;
    roi.height = faceWidth;

    std::cout << "FaceWidth=" << faceWidth << endl;

    /* crop face image */
    fcv::Mat cropA = img(roi);

    /* resize image for lcnn input */
    fcv::resize(cropA,faceImg,faceSize);

}




int main(int argc,char** argv)
{


}
