#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	kitten = ofImage("kitten.jpg");
	snow = ofImage("snow.jpg");
	fbo.allocate(kitten.getWidth() + snow.getWidth(), snow.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
	//generate some content
	fbo.begin();
	ofClear(0,0,0,255);
	kitten.draw(0,0);
	ofDrawBitmapString("OF Frame " + ofToString(ofGetFrameNum()),10,kitten.getHeight() + 30);
	ofTranslate(kitten.getWidth(), 0);
	ofSetColor((ofGetElapsedTimeMillis()/10)%255, (ofGetElapsedTimeMillis()/11)%255, (ofGetElapsedTimeMillis()/13)%255); 
	snow.draw(0,0);
	fbo.end();



	//Spout
	spout.sendTexture(kitten.getTexture(), "kitten");
	spout.sendTexture(snow.getTexture(), "snow");
	spout.sendTexture(fbo.getTexture(), "composition");
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255);
	ofClear(0,0,0,255);
	fbo.draw(0,0);

}

void ofApp::exit() {
	spout.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
