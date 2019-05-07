#include "ofApp.h"


#define COUNT 100
#define NOISE 0.01

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	this->camera.setFixUpDirectionEnabled(true);

    allFixtures.resize(2);
    int cnt = 0;
    for(auto & f:allFixtures){
        f.setup(cnt);
    }
    gui.setup();
    gui.setName("movingHead");
    gui.setPosition(20,20);
    gui.add(drawGrid.set("drawGrid",true));
    gui.add(showCursor.set("showCursor",true));
//    gui.add(allFixtures[0].fixture_parameters);
    for(auto & f:allFixtures){
        gui.add(f.fixture_parameters);
    }
    gui.loadFromFile("gui.xml");

	

	//this->solve();
}

//--------------------------------------------------------------
void ofApp::update(){
	camera.setCursorDrawEnabled(showCursor);

//    allFixtures[0].update();
    for(auto & f:allFixtures){
        f.update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(40, 10);

	camera.begin();
	{
		ofEnableDepthTest();

		if (drawGrid) {
			ofPushStyle();
			{
				ofSetColor(150);

				ofPushMatrix();
				{
					ofRotateDeg(180, 0, 1, 0);
					ofRotateDeg(90, 0, 0, 1);
					ofDrawGridPlane(1.0f, 10, true);
				}
				ofPopMatrix();
			}
			ofPopStyle();
		}

        allFixtures[0].drawTargets();
        for(auto & f:allFixtures){
            f.draw();
        }

		ofDisableDepthTest();
	}
	camera.end();

	gui.draw();
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
