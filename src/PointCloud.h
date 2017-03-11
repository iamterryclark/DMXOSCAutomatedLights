#pragma once
#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxGui.h"

#define kinectWidth 512
#define kinectHeight 424

class PointCloud
{
private:
	PointCloud();
	~PointCloud();

protected:

public:
	static PointCloud& get_pCloudInstance();
	static void destroy_pCloudInstance();

	void update();
	void draw();

	ICoordinateMapper* m_pCoordinateMapper; // For Skeleton Tracking
	DepthSpacePoint m_pDepthCoordinates[1920 * 1080]; //For Pointcloud depth across the width of the display screen

	ofMesh nativeWorld;
	ofMesh calculatedWorld;

	ofFloatPixels depthToWorldTable;
	ofTexture depthToWorldPreview;

	ofxPanel pcThreshGui;
	ofParameterGroup pcParams;
	ofParameter<float> minThresh, maxThresh;
	bool drawGui;
	int steps = 10;

	ofxKFW2::Device kinect;
};

