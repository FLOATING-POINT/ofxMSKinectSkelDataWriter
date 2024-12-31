#ifndef _MS_KINECT_DATA_WRITER
#define _MS_KINECT_DATA_WRITER

#include "ofMain.h"
#include "dataContainer.h"
//#include "ofxThread.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDING_TIME_SECOND 300
#define RECORDING_FPS 30
#define MAX_RECORDING_TIME (MAX_RECORDING_TIME_SECOND * RECORDING_FPS)

class ofxMSKinectDataWriter : protected ofThread {
private:
	FILE            *outFile;
	char            filename[256];
	bool            open;
	int             item_count;
	dataContainer * containers;
	double          startClock;


public:
	int frame;
	ofxMSKinectDataWriter(char *_filename);
	string startRecording();
	bool update(dataContainer *buffer);
	void stopRecording();
	string          filenameOut;
};

#endif

