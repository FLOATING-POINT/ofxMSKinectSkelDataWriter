#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofBackground(50,50,50);
    MSkinect.setup();

	//-----------------------------------------------------------------------------------------

	char buf[256];
	time_t timer;
	time(&timer);
	struct tm *t_st;
	t_st = localtime(&timer);

	sprintf(buf, "rec%dfps_%02d_%02d_%02d-%02d-%02d", RECORDING_FPS, t_st->tm_mon+1, t_st->tm_mday, t_st->tm_hour, t_st->tm_min, t_st->tm_sec);
	dataWriter      = new ofxMSKinectDataWriter(buf);
	countRecording  = 0;
	bRecording      = false;
	frameRecording  = 0;

	ofSetFrameRate(RECORDING_FPS);

	lastFileName    =  "No data saved yet";

    closeBtn.setup("Close the app",20, 40,155,25);
    ofAddListener(closeBtn.newHitEvent,this,&testApp::close);

    fullscreenBtn.setup("Exit fullscreen",185, 40,155,25);
    ofAddListener(fullscreenBtn.newHitEvent,this,&testApp::switchFullscreen);

    recordingBtn.setup("Start recording",20, 75,320,25);
    ofAddListener(recordingBtn.newHitEvent,this,&testApp::controlRecording);



    isFullscreen = true;

}

//--------------------------------------------------------------
void testApp::update(){

    fullscreenBtn.update();
    recordingBtn.update();


    MSkinect.update();

    //--------------------------------------------------------------------------------------

	if(bRecording) {

		char buf[512];
		double rec = (get_stamp_sec() - timeStampRecording);
		sprintf(buf, "now recording : %04d:%02d, %05d frame,", (int)rec / 60, (int)rec % 60, frameRecording);
		string str = buf;
		ofSetWindowTitle(str);
		frameRecording++;

		/*unsigned char * kinectDistancePixels    = MSkinect.GetDepthPixels();
		unsigned char * kinectRgbPixels         = MSkinect.GetColorPixels();

		for (int i=0; i<MSkinect.depthWidth; i++) {
			for (int j=0; j<MSkinect.depthHeight; j++) {
				int n = i + j * MSkinect.depthWidth;

				//container.distancePixels[n]     = kinectDistancePixels[n];

				container.rgbPixels[n * 3]      = kinectRgbPixels[n * 3];
				container.rgbPixels[n * 3 + 1]  = kinectRgbPixels[n * 3 + 1];
				container.rgbPixels[n * 3 + 2]  = kinectRgbPixels[n * 3 + 2];
			}
		}*/

		vector < ofVec3f > pnts1 = MSkinect.getAllPoints(0);
		for(int i = 0; i < pnts1.size(); i++){

            container.pos1[(i*3)+0] = pnts1[i].x;
            container.pos1[(i*3)+1] = pnts1[i].y;
            container.pos1[(i*3)+2] = pnts1[i].z;
        }

		//container.pos2 = MSkinect.getAllPoints(1);

		vector < ofVec3f > pnts2 = MSkinect.getAllPoints(1);
		for(int i = 0; i < pnts2.size(); i++){

            container.pos2[(i*3)+0] = pnts2[i].x;
            container.pos2[(i*3)+1] = pnts2[i].y;
            container.pos2[(i*3)+2] = pnts2[i].z;
        }



		bRecording = dataWriter->update( &container );
	} else {
		ofSetWindowTitle("waiting, press ! to start rec");
	}


	//-------------------------------------------------------------------------------------



}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(255,255,255);
    ofDrawBitmapString("NOTICE: This app requires a MS KInect camera installed and connected", 20,20);

    if(bRecording && MSkinect.nSkeletons == 0){
        if(ofInRange(ofGetFrameNum() % 8,0,3))ofSetColor(255,0,0);
        if(ofInRange(ofGetFrameNum() % 8,4,8))ofSetColor(0,0,0);

        ofDrawBitmapString("WARNING: No Skeletal data being tracked ", 360,36);
    }

    closeBtn.draw();
    fullscreenBtn.draw();
    recordingBtn.draw();



    ofSetColor(155,155,155);
    ofDrawBitmapString("Last file recorded: "+lastFileName, 25,120);

    if(bRecording){
        char buf[512];
		double rec = (get_stamp_sec() - timeStampRecording);
		sprintf(buf, "now recording : %04d:%02d secs, %05d frame,", (int)rec / 60, (int)rec % 60, frameRecording);
		string str = buf;
		ofDrawBitmapString("Recording: "+str, 25,150);
    } else {
        ofDrawBitmapString("Not Recording: ", 25,150);
    }

    ofPushMatrix();

    ofTranslate(20,170,0);
        MSkinect.draw();
    ofPopMatrix();

    MSkinect.drawAllLines();
    MSkinect.drawAllPoints();

    // draw limbs points

}
//--------------------------------------------------------------
void testApp::close(bool & hit){
    ofExit(1);
}
//--------------------------------------------------------------
void testApp::controlRecording(bool & hit){
    bRecording	= !bRecording;
    if(bRecording){
        filenames.push_back(dataWriter->startRecording());
        frameRecording	    = 0;
        timeStampRecording	= get_stamp_sec();
        recordingBtn.label  = "Stop recording";
    } else {
        dataWriter->stopRecording();
        bRecording = false;
        ofFilePath p;
        lastFileName = p.getCurrentWorkingDirectory()+"\data\\"+dataWriter->filenameOut;
        recordingBtn.label  = "Start recording";
    }
}
//--------------------------------------------------------------
void testApp::switchFullscreen(bool & hit){
    isFullscreen = !isFullscreen;
    isFullscreen ? fullscreenBtn.label = "Exit fullscreen" : fullscreenBtn.label = "Go fullscreen" ;
    ofToggleFullscreen();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if(key == 's') {
        bRecording	= !bRecording;
        if(bRecording){
            filenames.push_back(dataWriter->startRecording());
            frameRecording	    = 0;
            timeStampRecording	= get_stamp_sec();
        } else {
            dataWriter->stopRecording();
            bRecording = false;
        }
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
