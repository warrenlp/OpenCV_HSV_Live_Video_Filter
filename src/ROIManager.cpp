#include "../include/ROIManager.h"

#include <opencv2/core/core.hpp>

ROIManager* ROIManager::_instance = new ROIManager();
cv::Point2i ROIManager::mUpperLeft = cv::Point2i(0, 0);
cv::Point2i ROIManager::mLowerRight = cv::Point2i(0, 0);
bool ROIManager::mDrawRect = false;
bool ROIManager::mLBTNDOWN = false;

ROIManager::ROIManager(void)
{
}


ROIManager::~ROIManager(void)
{
	delete _instance;
}


void ROIManager::setUpperLeft(int x, int y)
{ 
	mUpperLeft = cv::Point2i(x, y);
	mLowerRight = cv::Point2i(0, 0);
	mDrawRect = false;
	mLBTNDOWN = true;
}

void ROIManager::setLowerRight(int x, int y)
{
	if (mLBTNDOWN)
	{
		mLowerRight = cv::Point2i(x, y);
		mLBTNDOWN = false;
	}
}

void ROIManager::stretchRect(int x, int y)
{
	if (mLBTNDOWN)
	{
		mLowerRight = cv::Point2i(x, y);
		mDrawRect = true;
	}
}

bool ROIManager::drawRect()
{
	return mDrawRect;
}
