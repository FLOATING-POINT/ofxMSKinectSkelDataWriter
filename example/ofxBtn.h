#ifndef _OFX_BTN
#define _OFX_BTN

#include "ofMain.h"
#include "ofEvents.h"


class ofxBtn{

	public:
        ~ofxBtn();
		void setup(string _label, float _x, float _y, float _w, float _h);
		void update();
		void draw();
		void setPos(float _x, float _y, float _w, float _h);
		bool hitTest(int tx, int ty);
		void _mousePressed(ofMouseEventArgs &e);
		void _mouseMoved(ofMouseEventArgs &e);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		float bw, bh, bx, by;
		int padding;
		string label;

		int textCol;
		int textColActive;
		int borderCol;
		int bgCol;

		bool hit;
		int hitID;
		bool active;

		ofEvent<bool> newHitEvent;
		ofEvent<int> newHitIDEvent;



};

#endif
