#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGrabCam.h"

#include "ofxCeres.h"

#include "oneFixture.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void solve();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
          
        ofxGrabCam camera;
    
    vector<oneFixture> allFixtures;
    
    ofParameter<bool> drawGrid; //{ "Draw grid", true };
    ofParameter<bool> showCursor; //{ "Show cursor", false };
		
		ofxPanel gui;
};
