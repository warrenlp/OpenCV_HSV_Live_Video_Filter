#include "../include/Histogram1D.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

Histogram1D::Histogram1D(void)
{
	histSize[0] = 256;
	hranges[0] = 0.0;
	hranges[1] = 255.0;
	ranges[0] = hranges;
	channels[0] = 0;
}


Histogram1D::~Histogram1D(void)
{
	delete [] ranges;
}

cv::Mat Histogram1D::getHistogram(const cv::Mat &image)
{
	cv::Mat hist;
	cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges);

	return hist;
}

cv::Mat Histogram1D::getHistogramImage(const cv::Mat &image)
{
	cv::MatND hist = getHistogram(image);

	// Get min and max values
	double maxVal = 0.0;
	double minVal = 0.0;

	cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

	// Image on which to display histogram
	cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));

	// Set highest point at 90% of bins
	int hpt = static_cast<int>(0.9*histSize[0]);

	for (int h=0; h<histSize[0]; ++h)
	{
		float binVal = hist.at<float>(h);
		int intensity = static_cast<int>(binVal*hpt/maxVal);

		// Draw lines for each bin value
		int height = getHistogramImageHeight();
		cv::line(histImg, cv::Point(h,height), cv::Point(h,height-intensity), cv::Scalar::all(0));
	}

	return histImg;
}

int Histogram1D::getHistogramImageHeight()
{
	return histSize[0];
}
