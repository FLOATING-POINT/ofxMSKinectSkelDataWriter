#include "ofxMSKinectDataWriter.h"

ofxMSKinectDataWriter::ofxMSKinectDataWriter(char *_filename) {

	containers  = new dataContainer[MAX_RECORDING_TIME];

	strncpy(filename, _filename, 256);
	item_count  = 1;
	frame       = 0;
	open        = false;
}

string ofxMSKinectDataWriter::startRecording() {
	if(open) {
		stopRecording();
	}
	char _filename[128];
	sprintf(_filename, "%s-%d.knct", filename, item_count);

	char buf[256];
	sprintf(buf, "%s%s", ofToDataPath("./").c_str(), _filename, item_count);

	char buf2[256];
	sprintf(buf2, "%s%s",  "", _filename, item_count);

	filenameOut = buf2;

	outFile = fopen(buf, "wb");
	if(outFile != NULL) {
		startClock = get_stamp_sec();
		open = true;
		cout << "ofxMSKinectDataWriter::startRecording() | file open success. : create file " << buf << " ..." << endl;
		cout << "ofxMSKinectDataWriter::startRecording() | start recording..." << endl;
		string outName = _filename;
		return outName;
	} else {
		cout << "ofxMSKinectDataWriter::startRecording() | file open error. : failed to open " << buf << " ..." << endl;
		return NULL;
	}
}

bool ofxMSKinectDataWriter::update(dataContainer *buffer) {
	if(open) {
		if(frame < MAX_RECORDING_TIME) {
			memcpy(&(containers[frame]), buffer, sizeof(dataContainer));
			containers[frame].stamp = get_stamp_sec() - startClock;

			frame++;
			return true;
		} else {
			stopRecording();
			return false;
		}
	}
	return false;
}

void ofxMSKinectDataWriter::stopRecording() {
	if(open) {
		int count = fwrite(containers, sizeof(dataContainer), frame, outFile);
		fclose(outFile);
		open = false;
		frame = 0;
		cout << "ofxMSKinectDataWriter::stopRecording() | stop recording #" << item_count << " ... " << count << " frame write." << endl;
		cout << "ofxMSKinectDataWriter::stopRecording() | " << containers[count - 1].stamp << "sec." << endl;
		item_count++;
	} else {
		cout << "ofxMSKinectDataWriter::stopRecording() | waiting. press '!' to start recording" << endl;
	}
}
