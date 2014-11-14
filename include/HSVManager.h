#pragma once

#include <opencv2/core/core.hpp>

#include <vector>
using namespace std;

class HSVManager
{
public:
	
	virtual ~HSVManager(void);
	static HSVManager* getInstance();

	cv::Mat generateHSVMask(const vector<cv::Mat>& HSV);
	void setHueUpper(int HueUpper);
	void setHueLower(int HueLower);
	void setSatUpper(int SatUpper);
	void setSatLower(int SatLower);
	void setValueUpper(int ValueUpper);
	void setValueLower(int ValueLower);
	void setMaskVideo(int maskVideo);
	inline int getHueUpper() const { return mHueUpper; };
	inline int getHueLower() const { return mHueLower; };
	inline int getSatUpper() const { return mSatUpper; };
	inline int getSatLower() const { return mSatLower; };
	inline int getValueUpper() const { return mValueUpper; };
	inline int getValueLower() const { return mValueLower; };
	
	bool showHueBars();
	bool showSatBars();
	bool showValueBars();
	bool showMaskVideo();

private:
	HSVManager(void);
	explicit HSVManager(HSVManager& manager) {}; // Hide copy constructor as private
	static HSVManager* _mInstance;

	int mHueUpper;
	int mHueLower;
	int mSatUpper;
	int mSatLower;
	int mValueUpper;
	int mValueLower;
	int mMaskVideo;

	bool bShowHue;
	bool bShowSat;
	bool bShowValue;
};

