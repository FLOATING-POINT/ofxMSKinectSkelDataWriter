
#pragma once

#include <stdlib.h>
#include <Windows.h>
#include "kinectWrapper.h"
#include "ofxMSKinect.h"
#include "ofxMSSkeletonDraw.h"

ofxMSKinect*        kinectDevice;
ofxMSKinectDevice*  firstDevice;
int padding         = 0;

ofxSkeletonRenderer* skeletonDrawer; // simple class to draw skeletons

void kinectWrapper::setup()
{
	//this will initialize everything we need for basic kinect functionality
	//this will also check if the device is connected and handle gracefully
	kinectDevice = new ofxMSKinect();
	kinectDevice->Init(SINGLE_DEVICE); //Pass MULTIPLE_DEVICES if you intend to use more than 1 kinect.


	if(kinectDevice->HasKinectDevice())
	{
		skeletonDrawer = new ofxSkeletonRenderer();

		firstDevice = kinectDevice->GetKinectDeviceDefault();

		printf("smoothing %f \n",firstDevice->GetSmoothing() );
		printf("jitter %f \n",firstDevice->GetJitterRadius() );

    //	NUI_TRANSFORM_SMOOTH_PARAMETERS inptr;    // get and set skeleton smoothing params
	//	firstDevice->GetTransformSmoothParamaters( &inptr );
	//	firstDevice->SetTransformSmoothParamaters( &inptr );
    //  or call individual functions for each of NUI_TRANSFORM_SMOOTH_PARAMETERS properties e.g
	//	firstDevice->SetSmoothing(f);
	//	float f  = firstDevice->GetSmoothing() ;

		// image res for RGB has to be this set by the NUI api .lower res returns failure
		bool isOk = firstDevice->StartRGB(IMAGE_RESOLUTION_640x480) ;

		// default depth resolution is 320x240
		isOk = firstDevice->StartDepth(IMAGE_RESOLUTION_320x240);

		//start tracking skeleton
		isOk = firstDevice->StartSkeletonTracking();

		kinectFailedToInit = false;

		depthWidth  = firstDevice->DepthSize.x;
        depthHeight = firstDevice->DepthSize.y;
	}
	else
	{
		kinectFailedToInit = true;
	}
}


void kinectWrapper::update()
{
	if(kinectFailedToInit)
		return;

	bool isOk   = firstDevice->UpdateRGBVideo();
	isOk        = firstDevice->UpdateDepth() ;
	isOk        = firstDevice->UpdateSkeletons();

	// or call the single  firstDevice->Update() convenience function ;
	nSkeletons = 0;
	if(firstDevice->HasSkeleton()){
        nSkeletons  = firstDevice->tracked_skeletons.size();
	}
}

//-------------------------------------------------------------
vector<ofVec3f> kinectWrapper::getHeadJoints(){

    headPoints.clear();

    ofVec3f headOne;
    ofVec3f headTwo;

    headOne.x = -1;
    headOne.y = -1;

    headTwo.x = -1;
    headTwo.y = -1;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        ofVec3f headpoint;
        //headpoint.y = ofGetScreenHeight();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);


                // head point is position number 3

                ofVec3f pos = skeletonData.SkeletonPositions[3];
                float fx=0;
                float fy=0;
                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x = fx * 640  + 0.5f;
                pos.y = fy * 480  + 0.5f;

                pos *= scaleSkeleton;

                pos.x += blurShaderOfx;
                pos.y += blurShaderOfy;

                headpoint = pos;


                skelIndex == 0 ? headOne = headpoint : headTwo = headpoint;


        }
    }

    headPoints.push_back(headOne);
    headPoints.push_back(headTwo);
    return headPoints;
}
//------------------------------------------------------------------------------
//-------------------------------------------------------------
vector<ofVec3f> kinectWrapper::getHandJoints(){

    handPoints.clear();

    ofVec3f handOneSkelOne;
    ofVec3f handTwoSkelOne;

    ofVec3f handOneSkelTwo;
    ofVec3f handTwoSkelTwo;

    handOneSkelOne.x = -1;
    handOneSkelOne.y = -1;

    handTwoSkelOne.x = -1;
    handTwoSkelOne.y = -1;

    handOneSkelTwo.x = -1;
    handOneSkelTwo.y = -1;

    handTwoSkelTwo.x = -1;
    handTwoSkelTwo.y = -1;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        ofVec3f handpointOne;
        ofVec3f handpointTwo;
        //headpoint.y = ofGetScreenHeight();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);


                // hand Points are 7 and 11

                // ---------------------------------------------------------

                ofVec3f posOne = skeletonData.SkeletonPositions[7];
                float fx_1 = 0;
                float fy_1 = 0;
                ofxMSKinectDevice::TransFormToDepth(posOne, &fx_1, &fy_1);

                posOne.x = fx_1 * 640  + 0.5f;
                posOne.y = fy_1 * 480  + 0.5f;

                posOne *= scaleSkeleton;

                posOne.x += blurShaderOfx;
                posOne.y += blurShaderOfy;

                handpointOne = posOne;

                // ---------------------------------------------------------

                ofVec3f posTwo = skeletonData.SkeletonPositions[11];
                float fx_2 = 0;
                float fy_2 = 0;
                ofxMSKinectDevice::TransFormToDepth(posTwo, &fx_2, &fy_2);

                posTwo.x = fx_2 * 640  + 0.5f;
                posTwo.y = fy_2 * 480  + 0.5f;

                posTwo *= scaleSkeleton;

                posTwo.x += blurShaderOfx;
                posTwo.y += blurShaderOfy;

                handpointTwo = posTwo;


                if(skelIndex == 0){
                    handOneSkelOne = handpointOne;
                    handOneSkelTwo = handpointTwo;
                } else {
                    handTwoSkelOne = handpointOne;
                    handTwoSkelTwo = handpointTwo;
                }


        }
    }

    handPoints.push_back(handOneSkelOne);
    handPoints.push_back(handOneSkelTwo);
    handPoints.push_back(handTwoSkelOne);
    handPoints.push_back(handTwoSkelTwo);
    return handPoints;
}


void kinectWrapper::draw()
{
	ofSetHexColor(0xffffff);

	if(kinectFailedToInit){
		ofSetHexColor(0xff0000);
		ofDrawBitmapString("No Kinect Device Found", padding, padding, 0);
		return;
	}



	//Draw depth
	firstDevice->DrawSilhouetteTexture(padding, padding, depthWidth, depthHeight);

	//ofSetHexColor(0xffffff);

	//Draw video (scale to size of depth for debug)
	firstDevice->DrawVidImage(padding + padding + depthWidth, padding, depthWidth, depthHeight);

//  Examples of drawing skeleton draw tracked and position only skels
	//skeletonDrawer->Draw(padding, firstDevice->DepthSize, firstDevice->HasSkeleton(),  &firstDevice->tracked_skeletons, &firstDevice->positiononly_skeletons);

	//skeletonDrawer->Draw(padding, firstDevice->DepthSize, firstDevice->HasSkeleton(),  &firstDevice->tracked_skeletons);
	//skeletonDrawer->DrawJoints(0, firstDevice->DepthSize, firstDevice->HasSkeleton(),  &firstDevice->tracked_skeletons );
//	skeletonDrawer->DrawLimbs(padding, firstDevice->DepthSize.x, firstDevice->DepthSize.y, firstDevice->HasSkeleton(),  &firstDevice->skeletons );

	//drawDebug();
	// or call firstDevice->Draw( padding ) to call convenience Draw function ;


}
//---------------------------------------------------------------------------------
unsigned char * kinectWrapper::GetDepthPixels(){
    return firstDevice->GetDepthPixels();
}
//---------------------------------------------------------------------------------
unsigned char * kinectWrapper::GetColorPixels(){
    return firstDevice->GetColorPixels();
}
//---------------------------------------------------------------------------------

void kinectWrapper::drawHeadPoints(){

    for(int i = 0; i < headPoints.size(); i++){
        ofSetHexColor(0xff0000);
        ofFill();
        if(headPoints[i].x > -1 && headPoints[i].y > -1) {

           ofRect(headPoints[i].x, headPoints[i].y, 10,10);
        }
	}

}
//---------------------------------------------------------------------------------
void kinectWrapper::drawAllPoints(){

    /* Points info
    0 = groin
    1 = stomach
    2 = neck
    3 = head
    4 = left shoulder
    5 = left elbow
    6 = left wrist
    7 = left hand
    8 = right shoulder
    9 = right elbow
    10 = right wrist
    11 = right hand
    12 = left hip
    13 = left knee
    14 = left ankle
    15 = left foot
    16 = right hip
    17 = right knee
    18 = right ankle
    19 = right toe

    */

    ofFill();

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();


        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

            for(int x = 0; x < 20; x++){

                // head point is position number 3

                ofVec3f pos = skeletonData.SkeletonPositions[x];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x = fx * 640  + 0.5f ;
                pos.y = fy * 480  + 0.5f + 240;

                ofSetHexColor(0xff0000);
                ofRect(pos.x-5, pos.y-5, 10,10);

                ofSetHexColor(0xffffff);
                ofDrawBitmapString(ofToString(x), pos.x-5 ,pos.y -5);

            }
        }
    }
}
//-------------------------------------------------------------------------
void kinectWrapper::drawAllLines(){

    ofFill();

    ofVec3f pos1;
    ofVec3f pos2;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();


        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

            for(int x = 1; x < 20; x++){

                // head point is position number 3

                pos1 = skeletonData.SkeletonPositions[x-1];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos1, &fx, &fy);

                pos1.x = fx * 640  + 0.5f ;
                pos1.y = fy * 480  + 0.5f + 240;

                pos2 = skeletonData.SkeletonPositions[x];
                ofxMSKinectDevice::TransFormToDepth(pos2, &fx, &fy);

                pos2.x = fx * 640  + 0.5f ;
                pos2.y = fy * 480  + 0.5f + 240;

                ofSetHexColor(0xff0000);
                if( x != 4 && x != 8 && x != 12 && x != 16) ofLine(pos1.x, pos1.y, pos2.x, pos2.y);

            }

            // across the shoulders
            pos1 = skeletonData.SkeletonPositions[4];
            float fx    = 0;
            float fy    = 0;

            ofxMSKinectDevice::TransFormToDepth(pos1, &fx, &fy);

            pos1.x = fx * 640  + 0.5f ;
            pos1.y = fy * 480  + 0.5f + 240;

            pos2 = skeletonData.SkeletonPositions[8];
            ofxMSKinectDevice::TransFormToDepth(pos2, &fx, &fy);

            pos2.x = fx * 640  + 0.5f ;
            pos2.y = fy * 480  + 0.5f + 240;
            ofSetHexColor(0xff0000);
            ofLine(pos1.x, pos1.y, pos2.x, pos2.y);

            // across the hips

            pos1 = skeletonData.SkeletonPositions[0];
            ofxMSKinectDevice::TransFormToDepth(pos1, &fx, &fy);

            pos1.x = fx * 640  + 0.5f ;
            pos1.y = fy * 480  + 0.5f + 240;

            pos2 = skeletonData.SkeletonPositions[12];
            ofxMSKinectDevice::TransFormToDepth(pos2, &fx, &fy);

            pos2.x = fx * 640  + 0.5f ;
            pos2.y = fy * 480  + 0.5f + 240;

            ofVec3f pos3 = skeletonData.SkeletonPositions[16];
            ofxMSKinectDevice::TransFormToDepth(pos3, &fx, &fy);

            pos3.x = fx * 640  + 0.5f ;
            pos3.y = fy * 480  + 0.5f + 240;

            ofSetHexColor(0xff0000);
            ofLine(pos1.x, pos1.y, pos2.x, pos2.y);
            ofLine(pos1.x, pos1.y, pos3.x, pos3.y);
        }
    }
}
//-------------------------------------------------------------------------
vector<ofVec3f> kinectWrapper::getAllPoints(int skelid){

    vector< ofVec3f > pnts;
    pnts.resize(20);

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                for(int i = 0; i < pnts.size(); i++){

                    // head point is position number 3

                    ofVec3f pos = skeletonData.SkeletonPositions[i];
                    float fx    = 0;
                    float fy    = 0;

                    ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                    pos.x       = fx * 640  + 0.5f;
                    pos.y       = fy * 480  + 0.5f;

                    pnts[i]     = pos;

                }
            }
        }
    }

    return pnts;

}

//-------------------------------------------------------------------------

void kinectWrapper::drawDebug(){

	ofSetHexColor(0x0);
	int dwidth = firstDevice->DepthSize.x;

	int leftOffset = padding + (dwidth * 2) + padding + padding;

	string name = firstDevice->GetDeviceName();
	int yPos    = padding;

	ostringstream ostr;

	ostr << "Device Name: " << name << "\r\n";

	ostr << "Connected :" << (firstDevice->IsConnected() ? "yes" : "no") << "\r\n";

	ostr << "RGB Size: " << firstDevice->VideoSize.x << " - " << firstDevice->VideoSize.y << "\r\n";

	ostr << "Depth Size: " << firstDevice->DepthSize.x << " - " << firstDevice->DepthSize.y << "\r\n";

	ostr << "Depth Has Player Info: " << (firstDevice->DepthHasPlayers() ? "yes" : "no") << "\r\n";

	bool hasSkeleton = firstDevice->HasSkeleton();
	ostr << "Has Skeletons: " << (hasSkeleton? "yes" : "no") << "\r\n";
	ofDrawBitmapString(ostr.str(), leftOffset, yPos, 0);
}
//---------------------------------------------------------------------
ofVec3f kinectWrapper::getGroinPoint(int skelid){ // point id 0

    int id = 0;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;
}
//---------------------------------------------------------------------
ofVec3f kinectWrapper::getStomachPoint(int skelid){ // point id 1

    int id = 1;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getNeckPoint(int skelid){ // point id 2

    int id = 2;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getHeadPoint(int skelid){ // point id 3

    int id = 3;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftShoulderPoint(int skelid){ // point id 4

    int id = 4;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftElbowPoint(int skelid){// point id 5

    int id = 5;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftWristPoint(int skelid){ // point id 6

    int id = 6;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftHandPoint(int skelid){ // point id 7

    int id = 7;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightShoulderPoint(int skelid){ // point id 8

    int id = 8;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightElbowPoint(int skelid){ // point id 9

    int id = 9;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightWristPoint(int skelid){ // point id 10

    int id = 10;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightHandPoint(int skelid){ // point id 11

    int id = 11;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftHipPoint(int skelid){ // point id 12

    int id = 12;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftKneePoint(int skelid){ // point id 13

    int id = 13;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftAnklePoint(int skelid){  // point id 14

    int id = 14;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getLeftFootPoint(int skelid){ // point id 15

    int id = 15;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightHipPoint(int skelid){ // point id 16

    int id = 16;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightKneePoint(int skelid){ // point id 17

    int id = 17;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightAnklePoint(int skelid){ // point id 18

    int id = 18;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}
//-------------------------------------------------------------------------
ofVec3f kinectWrapper::getRightToePoint(int skelid){ // point id 19

    int id = 19;
    ofVec3f pos;

    if(firstDevice->HasSkeleton()){

        int numOfSkelsTracked = firstDevice->tracked_skeletons.size();

        for(int skelIndex = 0; skelIndex < numOfSkelsTracked; skelIndex++){

            if(skelIndex == skelid){

                ofxMSKinectSkeleton skeletonData = firstDevice->tracked_skeletons.at(skelIndex);

                pos = skeletonData.SkeletonPositions[id];
                float fx    = 0;
                float fy    = 0;

                ofxMSKinectDevice::TransFormToDepth(pos, &fx, &fy);

                pos.x       = fx * 640  + 0.5f;
                pos.y       = fy * 480  + 0.5f;

            }
        }
    }
    return pos;

}

//-------------------------------------------------------------------------

long val;
void kinectWrapper::keyPressed(int key){
	if(key == OF_KEY_UP)
	{
		val++;
		firstDevice->SetCameraElevationAngle(val);
	}
	if(key == OF_KEY_DOWN)
	{
		val--;
		firstDevice->SetCameraElevationAngle(val);
	}
	if(key == 'm')
	{

	}
	//printf("Key %d\r\n", key);
}
//-------------------------------------------------------------------------

void kinectWrapper::keyReleased(int key){

}
//-------------------------------------------------------------------------

void kinectWrapper::exit(){
	if(kinectDevice != NULL)
		kinectDevice->Close();

	delete kinectDevice;
	kinectDevice = NULL;

	delete firstDevice;
	firstDevice = NULL;

	delete skeletonDrawer;
	skeletonDrawer = NULL;
}
