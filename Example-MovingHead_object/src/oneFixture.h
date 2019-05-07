//
//  oneFixture.h
//  Example-MovingHead
//
//  Created by Stephan Schulz on 2019-05-07.
//

#ifndef oneFixture_h
#define oneFixture_h

struct CoarseFineValue {
    uint8_t coarse;
    uint8_t fine;
    
    float getRatio() const {
        auto totalValue = ((uint16_t)coarse << 8) + (uint16_t)fine;
        return (float)totalValue / (float)numeric_limits<uint16_t>::max();
    }
};

struct PanTiltDMX {
    CoarseFineValue pan;
    CoarseFineValue tilt;
};

class oneFixture{
private:
    
public:
    
    int myID;
    
    ofParameterGroup fixture_parameters;
    ofParameter<glm::vec3> translation;
    ofParameter<glm::vec3> rotation;
    ofParameter<float> tiltOffset;
    
    ofParameter<bool> solve;
    
    vector<PanTiltDMX> panTiltDMXValues;
    vector<glm::vec3> targetPoints;
    vector<string> names;


    vector<glm::vec2> dmxValues;
    vector<glm::vec2> panTiltAngles;

    vector<ofColor> previewColors;
    
    glm::mat4 solvedTransform;
    


    
    void setup(int _id){
        myID = _id;
        
        fixture_parameters.setName("fixture "+ofToString(myID));

        
        fixture_parameters.add(translation.set( "Translation", glm::vec3(2.08, 0.78, 4.24), glm::vec3(-10), glm::vec3(+10)));
       fixture_parameters.add( rotation.set("Rotation", glm::vec3(0, -PI/2, 0), glm::vec3(-PI / 2), glm::vec3(+PI / 2)));
        fixture_parameters.add(tiltOffset.set( "Tilt offset", 0, -200, 200));
       fixture_parameters.add( solve.set("Solve", false));

        this->names.insert(this->names.end(), {
            "A"
            , "C"
            , "D"
            , "Y"
            , "E"
            , "F"
        });
        
        
        
        this->targetPoints.insert(this->targetPoints.end(),
                                  {
                                      {-2.20, -2.56, 0.78}
                                      , {3.76, 0.00, 0.78}
                                      , {3.76, -2.45, 0.78}
                                      , {2.35, 0.00, 0.78}
                                      , {3.76, -2.45, 2.32}
                                      , {3.76, -2.45, 0}
                                  });
        
//        this->panTiltDMXValues.insert(this->panTiltDMXValues.end(),
//                                      {
//                                          { {203 , 112} , {21 , 40} }
//                                          , { {161 , 47} , {22 , 20} }
//                                          , { {151 , 16} , {22 , 39} }
//                                          , { {0 , 0} , {22 , 19} }
//                                          , { {151 , 40} , {59 , 5} } // F and E are switched
//                                          , { {151 , 16} , {3 , 5} } // F and E are switched
//                                      }
//                                      );

        this->panTiltDMXValues.insert(this->panTiltDMXValues.end(),
                                      {
                                          { {uint8_t(ofRandom(255)) , 112} , {21 , 40} }
                                          , { {161 , 47} , {22 , uint8_t(ofRandom(255))} }
                                          , { {151 , 16} , {22 , 39} }
                                          , { {0 , 0} , {uint8_t(ofRandom(255)) , 19} }
                                          , { {151 , 40} , {59 , 5} } // F and E are switched
                                          , { {uint8_t(ofRandom(255)) , 16} , {3 , 5} } // F and E are switched
                                      }
                                      );

        
        // SketchUp to openFrameworks coordinates
        for (auto & targetPoint : targetPoints) {
            targetPoint = glm::vec3(targetPoint.x, targetPoint.z, -targetPoint.y);
        }
        
        //Translate DMX values into panTilt values
        // Pan is -270 to +270 degrees
        // Tilt is -135 to +135 degrees
        const auto dmxMaxValue = std::numeric_limits<uint16_t>::max();
        for (const auto & dmxValues : this->panTiltDMXValues) {
            this->panTiltAngles.emplace_back(
                                             ofMap(dmxValues.pan.getRatio(), 0, 1.0f, -270, +270)
                                             , ofMap(dmxValues.tilt.getRatio(), 0, 1.0f, -130, +130));
            
            ofColor previewColor(200, 100, 100);
            previewColor.setHueAngle(ofRandom(360));
            this->previewColors.push_back(previewColor);
        }
        


        
    }
    
    void update(){
        if (solve) {
            solveIt();
            solve = false;
        }
        
        solvedTransform = ofxCeres::VectorMath::createTransform(translation.get(), rotation.get());

    }
    
    //--------------------------------------------------------------
    void solveIt() {
        auto result = ofxCeres::Models::MovingHead::solve(targetPoints
                                                          , panTiltAngles
                                                          , ofxCeres::Models::MovingHead::Solution {
                                                              translation.get()
                                                              , rotation.get()
                                                              , tiltOffset.get()
                                                          });
        translation = result.solution.translation;
        rotation = result.solution.rotationVector;
        tiltOffset = result.solution.tiltOffset;
        
//        this->camera.lookAt(translation.get());
    }
    
    void drawTargets(){
        //draw target points
        auto previewColor = this->previewColors.data();
        {
            int index = 0; 
            for (const auto & point : this->targetPoints) {
                ofSetColor(*previewColor++);
                //                    ofSetColor(255,0,0);
                ofDrawSphere(point, 0.03);
                
                ofSetColor(255);
                ofDrawBitmapString(this->names[index], point);
                index++;
            }
        }
    }
    void draw(){
        
        ofPushStyle();
        {
//            drawTargets();
            
            //draw moving head
            ofNoFill();
            vector<glm::vec3> transmissionsInObjectSpace;
            ofPushMatrix();
            {
                ofMultMatrix(this->solvedTransform);
                
                //draw hardware
                ofSetColor(150);
                ofDrawAxis(0.4f);
                ofSetSphereResolution(6);
                ofDrawBox(glm::vec3(0, -0.35, 0), 0.5, 0.1, 0.4);
                ofPushMatrix();
                {
                    ofScale(0.6, 1, 0.6);
                    ofDrawSphere(0.2f);
                }
                ofPopMatrix();
                
                //draw rays
                ofSetColor(255);
                ofSetLineWidth(2.0f);
                auto previewColor = this->previewColors.data();
                {
                    int index = 0;
                    for (const auto & panTiltAngle : this->panTiltAngles) {
                        ofSetColor(*previewColor++);
                        auto transmission = ofxCeres::VectorMath::getObjectSpaceRayForPanTilt(panTiltAngle, tiltOffset.get());
                        
                        ofDrawLine(glm::vec3(), transmission);
                        transmissionsInObjectSpace.push_back(transmission);
                        
                        ofDrawBitmapString(this->names[index] , transmission);
                        index++;
                    }
                }
            }
            ofPopMatrix();
            
            //draw residuals
            ofPushStyle();
            {
                ofSetColor(100);
                ofSetLineWidth(1.0f);
                for (int i = 0; i < this->targetPoints.size(); i++) {
                    auto transmissionTipInWorldSpace = this->solvedTransform * glm::vec4(transmissionsInObjectSpace[i], 1.0f);
                    ofDrawLine((glm::vec3) transmissionTipInWorldSpace, this->targetPoints[i]);
                }
            }
            ofPopStyle();
        }
        ofPopStyle();
    }

};

#endif /* oneFixture_h */
