#include "TrackBarManager.h"

#include <string>
#include <map>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

TrackBarManager* TrackBarManager::_mInstance = NULL;

TrackBarManager* TrackBarManager::getInstance()
{
	if (_mInstance == NULL)
	{
		return _mInstance = new TrackBarManager;
	}
	else
	{
		return _mInstance;
	}
}

TrackBarManager::TrackBarManager(void)
{
}


TrackBarManager::~TrackBarManager(void)
{
	vector<int *>::const_iterator citer = mTrackBarCurVals.begin();
	vector<int *>::const_iterator cend = mTrackBarCurVals.end();
	for (; citer != cend; ++citer)
	{
		delete *citer;
	}

	delete _mInstance;
}

void TrackBarManager::createAllTrackbars(const string& windowName, 
		const vector<string>& trackBarNames,
		vector<int *>& trackBarCurVals,
		const vector<int>& trackBarHighVals,
		void (*trackBarHandler)(int, void*))
{
	// Check to make sure that all of the sizes are the same
	if (trackBarNames.size() == trackBarCurVals.size() && trackBarNames.size() == trackBarHighVals.size() && trackBarCurVals.size() == trackBarHighVals.size())
	{
		mWindowName = windowName;

		for (int i=0; i<trackBarNames.size(); ++i)
		{
			mTrackbarNames.push_back(trackBarNames.at(i));
			mTrackBarCurVals.push_back(trackBarCurVals.at(i));
			mTrackBarHighVals.push_back(trackBarHighVals.at(i));
		}
		mTrackBarHandler = trackBarHandler;
		createAllTrackbars();
	}
}

void TrackBarManager::createAllTrackbars()
{
	vector<string>::const_iterator citer = mTrackbarNames.begin();
	vector<string>::const_iterator cend = mTrackbarNames.end();
	
	for (int i = 0; citer != cend; ++citer, ++i)
	{
		const string name = *citer;
		const char* nameChar = citer->c_str();
		cv::createTrackbar(name, mWindowName, mTrackBarCurVals.at(i), mTrackBarHighVals.at(i), mTrackBarHandler, (void*)(nameChar));
	}
}
