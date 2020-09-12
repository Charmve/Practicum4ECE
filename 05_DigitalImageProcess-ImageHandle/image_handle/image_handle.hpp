
#ifndef __IMAGE_HANDLE_HPP__
#define __IMAGE_HANDLE_HPP__

#include "fastcv.hpp"

extern "C" {
#include <rockchip/rockchip_rga.h>
}
#include <rockchip/rockchip_isp.h>

using namespace fcv;
using namespace std;

#define MAGNIFICATION  1.5
#define REDUCTION 0.5

struct scene_processing {
    string wintitle_dst;
    void (*scene_processing_fun)(Mat src, Mat& dst);
};

enum SCENE_TYPE {
    HISTOGRAM_EQUALIZATION = 0,
    SMOOTH_FILTERING,
    EDGE_EXTRACTION,
    SHARPENING_FILTERING,
    BILINEAR_INTERPOLATION,
    SCALE_PART_AVERAGE,
    RELIEF,
    SKETCH,
    SCENE_MAX
};

void histogram_equalization (const Mat src, Mat& dst);
void median_smooth_flitering(const Mat src, Mat &dst);
void edge_extraction(const Mat src, Mat &dst);
void sharpen_flitering(const Mat src, Mat &dst);
void bilinear_interpolation(const Mat src, Mat &dst);
void scale_part_average (const Mat src, Mat &dst);
void relief(const Mat src, Mat &dst);
void sketch(const Mat src, Mat &dst);
#endif
