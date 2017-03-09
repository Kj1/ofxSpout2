#include "ofxSpout2Sender.h"

using namespace ofxSpout2;

Sender::Sender() {
	//ofAddListener(ofEvents().exit, this, &ofxSpout2::stopSpout);
}

//--------------------------------------------------------------
void Sender::sendTexture(ofTexture& t, string name)
{
    if (!t.isAllocated()) {
        ofLogNotice("ofxSpout2Sender", "texture to send it not allocated for sender name %s.", name.c_str());
        return;
    }

	int width = t.getWidth();
	int height = t.getHeight();

	//lookup the sender
	int i = ofFind(senderNameList, name); 

	//if the sender is not there
    bool buildFBO = false;
    bool appendFBO = false;
    if (i == senderNameList.size()) {
        char senderNameChars[256];
        strcpy_s(senderNameChars, name.c_str());
        SpoutSender* sender = new SpoutSender();
        bool init = sender->CreateSender(senderNameChars, width, height);
        if (init) {
            buildFBO = true;
            spoutSenderList.push_back(sender);
        }
        else {
            ofLogError("ofxSpout2 Sender creation failed");
        }
    }
    else if (width != senderWidthList[i] || height != senderHeightList[i]) {
        ofLogNotice("ofxSpout2", "Texture size changed. Updating sender.");
        char senderNameChars[256];
        strcpy_s(senderNameChars, name.c_str());
        spoutSenderList[i]->UpdateSender(senderNameChars, width, height); //  ReleaseSender();
        buildFBO = true;
    }

    if (buildFBO) {
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

        if (i >= senderNameList.size()) {
            // TODO: not very elegant. merge these things into a struct pack things into a map<Name,SenderDataStruct>
            senderFboList.push_back(senderFbo);
            senderWidthList.push_back(width);
            senderHeightList.push_back(height);
            senderNameList.push_back(name);

            //we cannot create sender and immediatly send texture; so return
            return;
        }
        else {
            senderFboList[i] = senderFbo;
            senderWidthList[i] = width;
            senderHeightList[i] = height;
            senderNameList[i] = name;
        }
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