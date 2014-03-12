#pragma once

#include <opencv2/core/core.hpp>

class Histogram1D
{
public:
	Histogram1D(void);
	virtual ~Histogram1D(void);

	cv::MatND getHistogram(const cv::Mat &image);
	cv::Mat getHistogramImage(const cv::Mat &image);
	inline int getHistogramImageHeight();

private:
	int histSize[1];
	float hranges[2];
	const float* ranges[1];
	int channels[1];
};

