#pragma once

#include "ofxMSKinectDataWriter.h"
#include <time.h>
#include "dataContainer.h"
#include "MSKinectWrapper.h"
#include "ofMain.h"
#include "ofxBtn.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void close(bool & hit);
        void controlRecording(bool & hit);
        void switchFullscreen(bool & hit);

		MSKinectWrapper          MSkinect;

        int                     frameRecording;
        double                  timeStampRecording;
        bool                    bRecording;
        ofxBtn                  recordingBtn;
        string                  lastFileName;

        int                     countRecording;
        vector <string>         filenames;

        dataContainer           container;

        ofxBtn                  closeBtn;

        bool                    isFullscreen;
        ofxBtn                  fullscreenBtn;




        /*inline double getrusage_sec() {

            struct rusage t;
            struct timeval tv;
            getrusage(RUSAGE_SELF, &t);
            tv = t.ru_utime;
            return tv.tv_sec + (double)tv.tv_usec * 1e-6;
        }*/


	private:
		void drawDebug();
		bool kinectFailedToInit;

		ofTrueTypeFont _font;
        ofxMSKinectDataWriter *dataWriter;

};
