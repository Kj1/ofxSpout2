// Spout : http://spout.zeal.co/

#pragma once

#include "ofMain.h"
#include "Spout.h"

class ofxSpout2
{
public:
	ofxSpout2();
	void sendTexture(ofTexture& t, string senderName);
	void exit();
	
	
private:

	//SENDER
	vector<SpoutSender*> spoutSenderList;		// A sender object
	vector<string>		senderNameList;
	vector<int>			senderWidthList;
	vector<int>			senderHeightList;
	vector<ofFbo>		senderFboList;


};