#include "Histogram1D.h"
#include "HSVHistogram.h"
#include "ROIManager.h"
#include "TrackBarManager.h"
#include "HSVManager.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>
#include <map>
#include <utility>

using namespace std;

#define HUE_UPPER_LIMIT 180
#define HUE_LOWER_LIMIT 0
#define SAT_UPPER_LIMIT 255
#define SAT_LOWER_LIMIT 0
#define VALUE_UPPER_LIMIT 255
#define VALUE_LOWER_LIMIT 0
#define MASK_VIDEO_UPPER_LIMIT 1
#define MASK_VIDEO_LOWER_LIMIT 0

HSVManager* HSVmanager = HSVManager::getInstance();

void ROISelector(int event, int x, int y, int flags, void* userdata)
{
	switch(event)
	{
	case CV_EVENT_LBUTTONDOWN:
		ROIManager::setUpperLeft(x, y);
		break;
	case CV_EVENT_LBUTTONUP:
		ROIManager::setLowerRight(x, y);
		break;
	case CV_EVENT_MOUSEMOVE:
		ROIManager::stretchRect(x, y);
		break;
	default:
		// Do nothing
		break;
	}
}

void TrackBarHandler(int pos, void* userdata)
{
	string name(reinterpret_cast<const char*>(userdata));

	if (name == "Hue Upper")
	{
		HSVmanager->setHueUpper(pos);
	}
	else if (name == "Hue Lower")
	{
		HSVmanager->setHueLower(pos);
	}
	else if (name == "Saturation Upper")
	{
		HSVmanager->setSatUpper(pos);
	}
	else if (name == "Saturation Lower")
	{
		HSVmanager->setSatLower(pos);
	}
	else if (name == "Value Upper")
	{
		HSVmanager->setValueUpper(pos);
	}
	else if (name == "Value Lower")
	{
		HSVmanager->setValueLower(pos);
	}
	else if (name == "Mask Video")
	{
		HSVmanager->setMaskVideo(pos);
	}

	cout << "pos: " << pos << " name: " << name << endl;
}

void createAllTrackbars(TrackBarManager * trackBarManager)
{
	vector<string> vTrackbarNames;
	vTrackbarNames.push_back("Hue Upper");
	vTrackbarNames.push_back("Hue Lower");
	vTrackbarNames.push_back("Saturation Upper");
	vTrackbarNames.push_back("Saturation Lower");
	vTrackbarNames.push_back("Value Upper");
	vTrackbarNames.push_back("Value Lower");
	vTrackbarNames.push_back("Mask Video");

	int TrackbarCurrent[] = {HUE_UPPER_LIMIT, HUE_LOWER_LIMIT, SAT_UPPER_LIMIT, SAT_LOWER_LIMIT, VALUE_UPPER_LIMIT, VALUE_LOWER_LIMIT, MASK_VIDEO_LOWER_LIMIT};
	vector<int> vTrackbarCurrentValue(TrackbarCurrent, TrackbarCurrent + sizeof(TrackbarCurrent)/sizeof(int));
	vector<int *> vTrackbarCurrent;

	vector<int>::const_iterator citer = vTrackbarCurrentValue.begin();
	vector<int>::const_iterator cend = vTrackbarCurrentValue.end();
	for (; citer != cend; ++citer)
	{
		vTrackbarCurrent.push_back(new int(*citer));
	}

	int TrackbarHighLimit[] = {HUE_UPPER_LIMIT, HUE_UPPER_LIMIT, SAT_UPPER_LIMIT, SAT_UPPER_LIMIT, VALUE_UPPER_LIMIT, VALUE_UPPER_LIMIT, MASK_VIDEO_UPPER_LIMIT};
	vector<int> vTrackbarHighLimit(TrackbarHighLimit, TrackbarHighLimit + sizeof(TrackbarHighLimit)/sizeof(int));

	const string windowName = "Hue/Saturation/Value Limits";
	cv::namedWindow(windowName, CV_WINDOW_NORMAL);
	trackBarManager->createAllTrackbars(windowName, vTrackbarNames, vTrackbarCurrent, vTrackbarHighLimit, TrackBarHandler);
}

int main( int argc, char** argv )
{
	cv::VideoCapture cap(0); // open the video file for reading

	if ( !cap.isOpened() )  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

	cout << "Frame per seconds : " << fps << endl;

	cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	cv::namedWindow("Saturation");
	cv::namedWindow("Hue");
	cv::namedWindow("Value");

	Histogram1D hist1D;
	cv::Mat frame;
	cv::Mat frameROI;
	cv::Mat HSV_full;
	cv::Mat HSV_ROI_full;
	vector<cv::Mat> hist_full(3);
	cv::Mat histSat;
	cv::Mat histHue;
	cv::Mat histValue;
	vector<cv::Mat> HSV(3);
	vector<cv::Mat> HSV_ROI(3);

	cv::setMouseCallback("MyVideo", ROISelector, NULL);

	TrackBarManager* trackBarManager = TrackBarManager::getInstance();
	createAllTrackbars(trackBarManager);

	while(1)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		cv::cvtColor(frame, HSV_full, CV_BGR2HSV);
		cv::split(HSV_full, HSV);

		if (ROIManager::drawRect())
		{
			cv::Point upperLeft = ROIManager::getUpperLeft();
			cv::Point lowerRight = ROIManager::getLowerRight();
			cv::Rect rect(upperLeft, lowerRight);
			if (rect.area() > 0) 
			{
				cv::rectangle(frame, rect, cv::Scalar(0,0,255));

				frameROI = frame(rect);
				cv::cvtColor(frameROI, HSV_ROI_full, CV_BGR2HSV);
				cv::split(HSV_ROI_full, HSV_ROI);
				histHue = hist1D.getHistogramImage(HSV_ROI.at(0));
				histSat = hist1D.getHistogramImage(HSV_ROI.at(1));
				histValue = hist1D.getHistogramImage(HSV_ROI.at(2));
			}
			else
			{
				histHue = hist1D.getHistogramImage(HSV.at(0));
				histSat = hist1D.getHistogramImage(HSV.at(1));
				histValue = hist1D.getHistogramImage(HSV.at(2));
			}
		}
		else
		{
			histHue = hist1D.getHistogramImage(HSV.at(0));
			histSat = hist1D.getHistogramImage(HSV.at(1));
			histValue = hist1D.getHistogramImage(HSV.at(2));
		}

		cv::cvtColor(histHue, histHue, CV_GRAY2BGR);
		cv::cvtColor(histSat, histSat, CV_GRAY2BGR);
		cv::cvtColor(histValue, histValue, CV_GRAY2BGR);

		bool bShowHSVMask(false);

		if (HSVmanager->showHueBars())
		{
			int hUpper = HSVmanager->getHueUpper();
			int histHeight = hist1D.getHistogramImageHeight();
			cv::line(histHue, cv::Point(hUpper,histHeight), cv::Point(hUpper,0), cv::Scalar(255,0,0));
			int hLower = HSVmanager->getHueLower();
			cv::line(histHue, cv::Point(hLower,histHeight), cv::Point(hLower,0), cv::Scalar(0,0,255));
			if (!bShowHSVMask)
			{
				bShowHSVMask = true;
			}
		}
		if (HSVmanager->showSatBars())
		{
			int sUpper = HSVmanager->getSatUpper();
			int histHeight = hist1D.getHistogramImageHeight();
			cv::line(histSat, cv::Point(sUpper,histHeight), cv::Point(sUpper,0), cv::Scalar(255,0,0));
			int sLower = HSVmanager->getSatLower();
			cv::line(histSat, cv::Point(sLower,histHeight), cv::Point(sLower,0), cv::Scalar(0,0,255));
			if (!bShowHSVMask)
			{
				bShowHSVMask = true;
			}
		}
		if (HSVmanager->showValueBars())
		{
			int vUpper = HSVmanager->getValueUpper();
			int histHeight = hist1D.getHistogramImageHeight();
			cv::line(histValue, cv::Point(vUpper,histHeight), cv::Point(vUpper,0), cv::Scalar(255,0,0));
			int vLower = HSVmanager->getValueLower();
			cv::line(histValue, cv::Point(vLower,histHeight), cv::Point(vLower,0), cv::Scalar(0,0,255));
			if (!bShowHSVMask)
			{
				bShowHSVMask = true;
			}
		}

		if (bShowHSVMask)
		{
			cv::Mat hsvMask = HSVmanager->generateHSVMask(HSV);
			cv::imshow("HSV Mask", hsvMask);
			if (HSVmanager->showMaskVideo())
			{
				cv::Mat tempFrame;
				frame.copyTo(tempFrame, hsvMask);
				frame = tempFrame;
				vector<cv::Mat> tempHSV(3);
				for (int i=0; i<3; ++i)
				{
					HSV.at(i).copyTo(tempHSV.at(i), hsvMask);
					HSV.at(i) = tempHSV.at(i);
				}
			}
		}

		cv::imshow("MyVideo", frame); //show the frame in "MyVideo" window
		cv::imshow("Hue", HSV.at(0));
		cv::imshow("Saturation", HSV.at(1));
		cv::imshow("Value", HSV.at(2));
		cv::imshow("Hue Histogram", histHue);
		cv::imshow("Saturation Histogram", histSat);
		cv::imshow("Value Histogram", histValue);

		if(cv::waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl; 
			break; 
		}
	}
	
	return 0;
}