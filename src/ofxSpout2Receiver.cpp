#pragma once

#include "ofxSpout2Receiver.h"

using namespace ofxSpout2;

Receiver::Receiver() {
	;
}

void Receiver::setup() {
	mReceiver = new SpoutReceiver;
	mbInitialized = false;
	SenderName[0] = 0;
}

void Receiver::showSenders() {
	mReceiver->SelectSenderPanel();
}

void Receiver::updateTexture() {
	if (!mbInitialized) {
		unsigned int width, height;
		if (mReceiver->CreateReceiver(SenderName, width, height, true)) {
			mTexture.allocate(width, height, GL_RGBA);
			mbInitialized = true;
			return;
		}
		else {
			ofLogWarning("ofxSpout", "No sender detected");
		}
	}

	else { // mbInitialized
		assert(mTexture.isAllocated() && "Texture not allocated but receiver initialized!");
		unsigned int preWidth = mTexture.getWidth();
		unsigned int preHeight = mTexture.getHeight();

		unsigned int width, height;
		if (mReceiver->ReceiveTexture(SenderName, width, height, mTexture.getTextureData().textureID, mTexture.getTextureData().textureTarget)) {
			if (width != preWidth || height != preHeight) { // in case of size change, reallocate
				mTexture.allocate(width, height, GL_RGBA);
				return;
			}
		}
		else {
			// receiving failed
			mReceiver->ReleaseReceiver();
			mbInitialized = false;
		}
	}
}

void Receiver::exit() {
	if(mReceiver != NULL && mbInitialized)
		mReceiver->ReleaseReceiver();
}