#pragma once

#include "ofMain.h"
#include "MS_NUI_Kinect_Interface.h"
#include "ofxMSKinectSkeleton.h"

class kinectWrapper
{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void keyReleased(int key);

		void drawHeadPoints();
		void drawAllPoints();
		void drawAllLines();

		vector<ofVec3f> getAllPoints(int skelid);

		ofVec3f getGroinPoint(int skelid); // point id 0
		ofVec3f getStomachPoint(int skelid); // point id 1
		ofVec3f getNeckPoint(int skelid); // point id 2
		ofVec3f getHeadPoint(int skelid); // point id 3
		ofVec3f getLeftShoulderPoint(int skelid); // point id 4
		ofVec3f getLeftElbowPoint(int skelid); // point id 5
		ofVec3f getLeftWristPoint(int skelid); // point id 6
		ofVec3f getLeftHandPoint(int skelid); // point id 7
		ofVec3f getRightShoulderPoint(int skelid); // point id 8
		ofVec3f getRightElbowPoint(int skelid); // point id 9
		ofVec3f getRightWristPoint(int skelid); // point id 10
		ofVec3f getRightHandPoint(int skelid); // point id 11
		ofVec3f getLeftHipPoint(int skelid); // point id 12
		ofVec3f getLeftKneePoint(int skelid); // point id 13
		ofVec3f getLeftAnklePoint(int skelid); // point id 14
		ofVec3f getLeftFootPoint(int skelid); // point id 15
		ofVec3f getRightHipPoint(int skelid); // point id 16
		ofVec3f getRightKneePoint(int skelid); // point id 17
		ofVec3f getRightAnklePoint(int skelid); // point id 18
		ofVec3f getRightToePoint(int skelid); // point id 19


		unsigned char * GetDepthPixels();
		unsigned char * GetColorPixels();

		vector<ofVec3f> getHeadJoints();
		vector<ofVec3f> getHandJoints();

		vector<ofVec3f> headPoints;
		vector<ofVec3f> handPoints;

		float scaleSkeleton;
		float blurShaderOfx,blurShaderOfy;

		int nSkeletons;
        int depthWidth;
        int depthHeight;

	private:
		void drawDebug();
		bool kinectFailedToInit;
};
