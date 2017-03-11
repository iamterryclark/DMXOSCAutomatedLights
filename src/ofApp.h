#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "PointCloud.h"
#include "ofxGui.h"
#include "ofxOsc.h"

#define bodies 6
#define HOST "localhost"
#define PORT 7000

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

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

	bool drawpoint;
	vector<ofVec3f> pathOfPoints;
	int timer;

	ofEasyCam cam;

	ofVec3f rightHandInWorld = ofVec3f(0, 0, 6);
	ofVec3f middleInWorld;
	PointCloud* pc;

	ofVec3f lightLeft = ofVec3f(-1.8, 2.5, 0);
	ofVec3f lightRight = ofVec3f(1.8, 2.5, 0);
	float angleLeftLightPan, angleRightLightPan;
	float angleLeftLightTilt, angleRightLightTilt;

	ofShader visualShader;

	ofxOscSender rotationLeft, rotationRight;
};