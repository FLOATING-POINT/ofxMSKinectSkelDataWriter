#include "ofxBtn.h"

//--------------------------------------------------------------
void ofxBtn::setup(string _label, float _x, float _y, float _w, float _h){

    bw          = _w;
    bh          = _h;
    bx          = _x;
    by          = _y;
    label       = _label;
    padding     = 4;

    textCol         = 0x666666;
    textColActive   = 0xff0000;
    borderCol       = 0x000000;
    bgCol           = 0x333333;

    hit         = true;
    active      = true;

    ofAddListener(ofEvents.mousePressed, this, &ofxBtn::_mousePressed);
    ofAddListener(ofEvents.mouseMoved, this, &ofxBtn::_mouseMoved);


}

//--------------------------------------------------------------
void ofxBtn::update(){
}

//--------------------------------------------------------------
void ofxBtn::draw(){

    ofSetHexColor(bgCol);
    ofFill();
    ofRect(bx+padding,by+padding,bw-(padding*2),bh-(padding*2));

    ofNoFill();
    ofSetHexColor(borderCol);
    ofRect(bx,by,bw,bh);

    if(!active) {
        ofSetHexColor(textCol);
    } else {
        ofSetHexColor(textColActive);
    }
    ofDrawBitmapString(label,bx+(padding*2),by+padding+(bh-12));

}
//----------------------------------------------------------------------------
void ofxBtn::setPos(float _x, float _y, float _w, float _h){
    bw          = _w;
    bh          = _h;
    bx          = _x;
    by          = _y;
}
//----------------------------------------------------------------------------
void ofxBtn::_mousePressed(ofMouseEventArgs &e) {
	int x = e.x;
	int y = e.y;
	int button = e.button;

	if(hitTest(x, y)) {
	    ofNotifyEvent(newHitEvent,hit,this);
	    ofNotifyEvent(newHitIDEvent,hitID,this);
	}

}
//----------------------------------------------------------------------------
void ofxBtn::_mouseMoved(ofMouseEventArgs &e) {

    if(hitTest(e.x, e.y)){
       //glutSetCursor(GLUT_CURSOR_NONE);
    } else {
        //glutSetCursor();
    }

}
//----------------------------------------------------------------------------
bool ofxBtn::hitTest(int tx, int ty) {

    bool hit = false;
    if(((tx > bx) && (tx < bx + bw) && (ty > by) && (ty < by + bh))){
       hit = true;

    }
    return hit;
}
//--------------------------------------------------------------
void ofxBtn::keyPressed(int key){

}

//--------------------------------------------------------------
void ofxBtn::keyReleased(int key){

}

//--------------------------------------------------------------
void ofxBtn::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofxBtn::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxBtn::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxBtn::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofxBtn::windowResized(int w, int h){

}
ofxBtn::~ofxBtn(){
}

