#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	kinect.open();
	kinect.initBodySource();
	kinect.initColorSource();
	kinect.initDepthSource();
	calculatedWorld.setMode(OF_PRIMITIVE_POINTS);
	//glPointSize(2);


	pcThreshGui.setup("ThresholdGUI");
	pcThreshGui.add(minThresh.set("Min Threshold:", 0, 0, 1));
	pcThreshGui.add(maxThresh.set("Max Threshold", 1, 0, 1));

	cam.rotate(180, 0, 1, 0);

	rightHandInWorld = ofVec3f(0,0,0);
	visualShader.load("vertShader.vert", "fragShader.frag");

	cam.setNearClip(-2);
	cam.setFarClip(2);

	rotationLeft.setup(HOST, PORT);
	rotationRight.setup(HOST,PORT);
}

//--------------------------------------------------------------
void ofApp::update() {
	kinect.update();
	if (kinect.isFrameNew()) {


		auto depth = kinect.getDepthSource();

		//This part will only happen once by using {} which acts like a data structure 'struct'
		//Reference: http://stackoverflow.com/questions/17701197/how-to-run-code-inside-a-loop-only-once-without-external-flag

		{
			depth->getDepthToWorldTable(depthToWorldTable);
			depthToWorldPreview.loadData(depthToWorldTable);
		}

		//build a mesh using the depthToWorldTable
		{
			auto size = depth->getWidth() * depth->getHeight();

			auto depthPixel = depth->getPixels().getData();
			auto depthToWorldRay = (ofVec2f*)depthToWorldTable.getData();

			this->calculatedWorld.clear();


			for (int i = 0; i < size; i++) {
				// Access ptr to change it with *depthPixel
				//Normalise the value from mm to m by dividing the depthPixel by 1000.0f
				float z = (float)*depthPixel / 1000.0f;
				int scale = 200;
				ofVec3f vertex{ depthToWorldRay->x * z,
					depthToWorldRay->y * z,
					z };

				calculatedWorld.addVertex(vertex);
				depthPixel++;
				depthToWorldRay++;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(0);
	cam.begin();
	int scale = ofGetWidth() / 2 - 100;
	ofScale(scale,scale,scale);
	

	//Check skeletons and grab right hand

	for (int i = 0; i < bodies; i++) {
		//Get the bodies tracked, although i am unable to just get one body index [0] at the moment
		auto b = this->kinect.getBodySource()->getBodies()[i];

		// check to see if a body is tracked...
		if (b.tracked) {
			rightHandInWorld = b.joints[JointType_HandRight].getPositionInWorld();
			middleInWorld = b.joints[JointType_SpineMid].getPositionInWorld();
		}
	}

	//cout << rightHandInWorld.z << endl;

	//The z psace check
	if (rightHandInWorld.z < 1.5) {
		timer++;
		if (timer > 10) {
			//if (ofGetFrameNum() % 10 == 0) {
				//cout << "oh Yea" << endl;
				pathOfPoints.push_back(rightHandInWorld);
			//}
		}
	}
	else {
		timer = 0;
		cout << "oh No" << timer << endl;
	}

	//Draw PointCloud
	ofSetColor(ofColor::blueSteel);
	this->calculatedWorld.drawVertices();

	//Draw skeleton path points
	ofSetColor(255, 0, 0, 0.5);
	ofDrawCircle(rightHandInWorld, 0.02);

	for (int i = 0; i < pathOfPoints.size(); i++) {
		ofSetColor(ofColor::seaGreen);
		ofDrawCircle(pathOfPoints[i], 0.008);
	}

	//DrawLights

	//Left Light

	ofVec3f distLeft = lightLeft - middleInWorld;
	
	angleLeftLightTilt = ofRadToDeg(atan(distLeft.z / distLeft.x));
	angleLeftLightPan = ofRadToDeg(atan(distLeft.x / distLeft.y));

	ofPushMatrix();

		ofTranslate(lightLeft);
		ofRotate(angleLeftLightPan,  1, 0, 0);
		ofRotate(angleLeftLightTilt, 0, 1, 0);
	
		//Fixture
		ofSetColor(ofColor::orangeRed);
		ofDrawCircle(0, 0, 0.2);

		//Fixture Axis
		ofSetColor(ofColor::indianRed);
		ofDrawLine(0, 0, 0, 0, 0, 1);
		ofSetColor(ofColor::forestGreen);
		ofDrawLine(0, 0, 0, 0, 1, 0);
		ofSetColor(ofColor::blue);
		ofDrawLine(0, 0, 0, 1, 0, 0);
	ofPopMatrix();

	//Right Light
	ofVec3f distRight = lightRight - middleInWorld;

	angleRightLightTilt = atan(distRight.z / distRight.x);
	angleRightLightPan = atan(distRight.x / distRight.y);

	ofPushMatrix();
		ofTranslate(lightRight);
		ofRotate(angleRightLightPan, 0, 1, 0);
		ofRotate(angleRightLightTilt, 1, 0, 0);
	
		//Fixture
		ofSetColor(ofColor::orangeRed);
		ofDrawCircle(0,0, 0.2);
	
		//Fixture axis
		ofSetColor(ofColor::indianRed);
		ofDrawLine(0, 0, 0, 0, 0, 1);
		ofSetColor(ofColor::forestGreen);
		ofDrawLine(0, 0, 0, 0, 1, 0);
		ofSetColor(ofColor::blue);
		ofDrawLine(0, 0, 0, -1, 0, 0);
	ofPopMatrix();

	ofxOscMessage m;
	m.setAddress("/LeftLight");
	m.addFloatArg(angleLeftLightPan);
	m.addFloatArg(angleLeftLightTilt);
	rotationLeft.sendMessage(m, false)

	m.setAddress("/RightLight");
	m.addFloatArg(angleRightLightPan);
	m.addFloatArg(angleRightLightTilt);

	rotationLeft.sendMessage(m, false);
	
	//cout << rightHandInWorld << endl;

	visualShader.begin();
	visualShader.setUniform1f("time", ofGetElapsedTimef());
	visualShader.setUniform2f("mouse", ofVec2f(ofGetMouseX(), ofGetMouseY()));
	visualShader.setUniform2f("resolution", ofVec2f(ofGetWidth(), ofGetHeight()));
	ofDrawRectangle(2,1, -1, -4, -2);
	visualShader.end();

	cam.end();

	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case ' ':
		cout << pathOfPoints.size() << endl;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
