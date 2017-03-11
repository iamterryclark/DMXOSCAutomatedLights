#include "PointCloud.h"

PointCloud::PointCloud() {
	calculatedWorld.setMode(OF_PRIMITIVE_POINTS);
	//glPointSize(2);


	pcThreshGui.setup("ThresholdGUI");
	pcThreshGui.add(minThresh.set("Min Threshold:", 0, 0, 1));
	pcThreshGui.add(maxThresh.set("Max Threshold", 1, 0, 1));
	
	cout << "pointCloud init" << endl;
}


PointCloud::~PointCloud() {
}

PointCloud& PointCloud::get_pCloudInstance() {
	static PointCloud *pointCloud = NULL;

	if (pointCloud == NULL) {
		pointCloud = new PointCloud();
		cout << "Pointcloud Created" << endl;
	}

	return *pointCloud;
}

void PointCloud::destroy_pCloudInstance() {
	PointCloud* pointCloud = &get_pCloudInstance();

	if (pointCloud != NULL) delete pointCloud;

	cout << "PointCLoud Destroyed" << endl;

}

//As at 05/03/2017
//I managed to integrate https://github.com/elliotwoods/ofxKinectForWindows2/example this example into this project
//I found that by doing this I was able to obtain a corrected pointcloud in 3d world view.
//This would now mean that  I can use this in order to create visuals and map this
//perfectly to the subjects form/shape.
void PointCloud::update() {
	
}


void PointCloud::draw() {
		
}

