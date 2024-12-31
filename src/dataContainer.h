#ifndef _DATA_CONTAINER
#define _DATA_CONTAINER

#include "ofMain.h"

#include <sys/time.h>
#define NPOINTS 4

class dataContainer {
public:
	double stamp;

	float pos1[NPOINTS*2];


};

inline double get_stamp_sec() {
//	struct rusage t;
//	struct timeval tv;
//	getrusage(RUSAGE_SELF, &t);
//	tv = t.ru_utime;
//	return tv.tv_sec + (double)tv.tv_usec * 1e-6;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

#endif
