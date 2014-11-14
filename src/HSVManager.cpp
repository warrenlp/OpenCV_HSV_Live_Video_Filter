#include "../include/HSVManager.h"

#include <stdlib.h>
#include <vector>
#include <utility>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

HSVManager* HSVManager::_mInstance = NULL;

HSVManager::HSVManager(void)
	:mHueUpper(180),
	mHueLower(0),
	mSatUpper(255),
	mSatLower(0),
	mValueUpper(255),
	mValueLower(0),
	mMaskVideo(0),
	bShowHue(false),
	bShowSat(false),
	bShowValue(false)
{
	// Do nothing
}


HSVManager::~HSVManager(void)
{
	delete _mInstance;
}

HSVManager* HSVManager::getInstance()
{
	if (_mInstance == NULL)
	{
		_mInstance = new HSVManager;
	}
	
	return _mInstance;
}

cv::Mat HSVManager::generateHSVMask(const vector<cv::Mat>& HSV)
{
	cv::Mat hsvMat(HSV.at(0).size(), HSV.at(0).type());
	vector<cv::Mat> HSVMasks(3);

	vector<pair<int, int> > vHSVValues;
	vHSVValues.push_back(make_pair(getHueLower(), getHueUpper()));
	vHSVValues.push_back(make_pair(getSatLower(), getSatUpper()));
	vHSVValues.push_back(make_pair(getValueLower(), getValueUpper()));

	for (int i=0; i<3; ++i)
	{
		if ( vHSVValues.at(i).first <= vHSVValues.at(i).second)
		{
			cv::inRange(HSV.at(i), cv::Scalar(vHSVValues.at(i).first), cv::Scalar(vHSVValues.at(i).second), HSVMasks.at(i));
		}
		else
		{
			cv::inRange(HSV.at(i), cv::Scalar(vHSVValues.at(i).second), cv::Scalar(vHSVValues.at(i).first), HSVMasks.at(i));
			cv::bitwise_not(HSVMasks.at(i), HSVMasks.at(i));
		}
		if (i > 0)
		{
			cv::bitwise_and(HSVMasks.at(i-1), HSVMasks.at(i), HSVMasks.at(i));
		}
	}

	//cv::namedWindow("Hue Mask");
	//cv::namedWindow("Sat Mask");
	//cv::namedWindow("Value Mask");
	//cv::imshow("Hue Mask", HSVMasks.at(0));
	//cv::imshow("Sat Mask", HSVMasks.at(1));
	//cv::imshow("Value Mask", HSVMasks.at(2));

	return HSVMasks.at(2);
}

void HSVManager::setHueUpper(int HueUpper)
{
	mHueUpper = HueUpper;
	bShowHue = true;
}

void HSVManager::setHueLower(int HueLower)
{
	mHueLower = HueLower;
	bShowHue = true;
}

bool HSVManager::showHueBars()
{
	return bShowHue;
}

void HSVManager::setSatUpper(int SatUpper)
{
	mSatUpper = SatUpper;
	bShowSat = true;
}

void HSVManager::setSatLower(int SatLower)
{
	mSatLower = SatLower;
	bShowSat = true;
}

bool HSVManager::showSatBars()
{
	return bShowSat;
}

void HSVManager::setValueUpper(int ValueUpper)
{
	mValueUpper = ValueUpper;
	bShowValue = true;
}

void HSVManager::setValueLower(int ValueLower)
{
	mValueLower = ValueLower;
	bShowValue = true;
}

bool HSVManager::showValueBars()
{
	return bShowValue;
}

void HSVManager::setMaskVideo(int maskVideo)
{
	mMaskVideo = maskVideo;
}

bool HSVManager::showMaskVideo()
{
	return (mMaskVideo == 1);
}
