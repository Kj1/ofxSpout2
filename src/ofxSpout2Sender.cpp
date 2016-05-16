#include "ofxSpout2Sender.h"

using namespace ofxSpout2;

Sender::Sender() {
	//ofAddListener(ofEvents().exit, this, &ofxSpout2::stopSpout);
}

//--------------------------------------------------------------
void Sender::sendTexture(ofTexture& t, string name)
{
	int width = t.getWidth();
	int height = t.getHeight();

	//lookup the sender
	int i = ofFind(senderNameList, name); 

	//if the sender is not there
	if (i == senderNameList.size()) {
		char senderNameChars[256];
		strcpy_s(senderNameChars, name.c_str());
		SpoutSender* sender = new SpoutSender();
		bool init = sender->CreateSender(senderNameChars, width, height);
	
		if (init) {
			ofLogNotice("ofxSpout2 Sender created");
			//init the fbo
			ofFbo senderFbo;
			ofFbo::Settings s;
			s.width = width;
			s.height = height;
			s.internalformat = GL_RGBA;
			s.textureTarget = GL_TEXTURE_2D;
			s.useDepth = false;
			s.useStencil = false;
			s.minFilter = GL_LINEAR;
			s.maxFilter = GL_LINEAR;
			s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
			s.wrapModeVertical = GL_CLAMP_TO_EDGE;
			s.numSamples = 0;
			senderFbo.allocate(s);
			senderFboList.push_back(senderFbo);
			senderWidthList.push_back(width);
			senderHeightList.push_back(height);
			senderNameList.push_back(name);
			spoutSenderList.push_back(sender);
		} else {
			ofLogError("ofxSpout2 Sender creation failed");
		}
		//we cannot create sender and immediatly send texture; so return
		return;
	}


	if (i >= 0 && i < senderNameList.size())
	{		
		ofPushMatrix();
		ofPushStyle();
		senderFboList[i].begin();
		ofClear(0);
		ofSetColor(255);
		t.draw(0,0,width,height);
		senderFboList[i].end();
		ofPopStyle();
		ofPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);  //
		spoutSenderList[i]->SendTexture(senderFboList[i].getTextureReference().getTextureData().textureID, GL_TEXTURE_2D, senderWidthList[i], senderHeightList[i], false); 
	}
}

//--------------------------------------------------------------
void Sender::exit()
{
	for (int i = 0; i <spoutSenderList.size();i++) {
		spoutSenderList[i]->ReleaseSender(); 
	}
	spoutSenderList.clear();
}