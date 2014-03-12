#pragma once

#include <vector>
#include <string>
using namespace std;

class TrackBarManager
{
public:
	
	virtual ~TrackBarManager(void);

	static TrackBarManager* getInstance();

	void createAllTrackbars(const string& windowName, 
		const vector<string>& trackBarNames,
		vector<int *>& mTrackBarCurVals,
		const vector<int>& mTrackBarHighVals,
		void (*trackBarHandlers)(int, void*));

	void createAllTrackbars();

private:
	TrackBarManager(void);
	static TrackBarManager* _mInstance;

	// Stores the name of the trackbar
	string mWindowName;
	vector<string> mTrackbarNames;
	vector<int*> mTrackBarCurVals;
	vector<int> mTrackBarHighVals;
	void (*mTrackBarHandler)(int, void*);
};

