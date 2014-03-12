#pragma once

#include <opencv2/core/core.hpp>

class ROIManager
{
public:
	virtual ~ROIManager(void);

	static void setUpperLeft(int x, int y);
	static void setLowerRight(int x, int y);
	static cv::Point getUpperLeft() { return mUpperLeft; }
	static cv::Point getLowerRight() { return mLowerRight; }
	static void stretchRect(int x, int y);
	static bool drawRect();

private:
	ROIManager(void);
	static ROIManager* _instance;
	static cv::Point2i mUpperLeft;
	static cv::Point2i mLowerRight;
	static bool mDrawRect;
	static bool mLBTNDOWN;
};

