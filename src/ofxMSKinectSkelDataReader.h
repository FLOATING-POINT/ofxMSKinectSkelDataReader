#pragma once

#include "ofMain.h"
#include "dataContainer.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

class ofxMSKinectSkelDataReader {
	private :
	FILE            *rawFile;
	dataContainer   *container;
	double          sec_to_max_stamp;
	double          maxClock;
	double          relativeMaxClock;
	double          startClockOffset;
	double          prevClock;


	float * pos1;
	float * pos2;

public :

	bool open;
	bool isStarted;
	ofxMSKinectSkelDataReader(const char *filename);
	bool start();
	void stop();
	void resume();

	void updatePnts();
	void drawLines(int skelID);
	void drawPoints(int skelID);
	void drawCirclePoints(int skelID);
	void drawLimbPoints(int skelID);

	double readTo();
	void shift(int ds);
	void nextFrame();
	void prevFrame();
    void doFrameMove();

	int             currentFrame;
	int             maxFrame;

	float * getPoints(int skelID);

    ofVec3f getPoint(int skelid, int pntID);
    ofVec3f getGroinPoint(int skelid); // point id 0
    ofVec3f getStomachPoint(int skelid); // point id 1
    ofVec3f getNeckPoint(int skelid); // point id 2
    ofVec3f getHeadPoint(int skelid); // point id 3
    ofVec3f getLeftShoulderPoint(int skelid); // point id 4
    ofVec3f getLeftElbowPoint(int skelid); // point id 5
    ofVec3f getLeftWristPoint(int skelid); // point id 6
    ofVec3f getLeftHandPoint(int skelid); // point id 7
    ofVec3f getRightShoulderPoint(int skelid); // point id 8
    ofVec3f getRightElbowPoint(int skelid); // point id 9
    ofVec3f getRightWristPoint(int skelid); // point id 10
    ofVec3f getRightHandPoint(int skelid); // point id 11
    ofVec3f getLeftHipPoint(int skelid); // point id 12
    ofVec3f getLeftKneePoint(int skelid); // point id 13
    ofVec3f getLeftAnklePoint(int skelid); // point id 14
    ofVec3f getLeftFootPoint(int skelid); // point id 15
    ofVec3f getRightHipPoint(int skelid); // point id 16
    ofVec3f getRightKneePoint(int skelid); // point id 17
    ofVec3f getRightAnklePoint(int skelid); // point id 18
    ofVec3f getRightToePoint(int skelid); // point id 19

    ofVec3f getLimbPoint(int skelid, int posID1, int posID2);
    ofVec3f getLimbPairPoint(int skelid, int posID1, int posID2,int posID3);

    ofVec3f getRightArmPoint(int skelid);
    ofVec3f getRightUpperArmPoint(int skelid);
    ofVec3f getRightLowerArmPoint(int skelid);

    ofVec3f getLeftArmPoint(int skelid);
    ofVec3f getLeftUpperArmPoint(int skelid);
    ofVec3f getLeftLowerArmPoint(int skelid);

    ofVec3f getRightLegPoint(int skelid);
    ofVec3f getRightUpperLegPoint(int skelid);
    ofVec3f getRightLowerLegPoint(int skelid);

    ofVec3f getLeftLegPoint(int skelid);
    ofVec3f getLeftUpperLegPoint(int skelid);
    ofVec3f getLeftLowerLegPoint(int skelid);

    ofVec3f                 headPntS1;

    ofVec3f                 rightArmPntS1;
    ofVec3f                 rightUpperArmPntS1;
    ofVec3f                 rightLowerArmPntS1;

    ofVec3f                 rightLegPntS1;
    ofVec3f                 rightUpperLegPntS1;
    ofVec3f                 rightLowerlegPntS1;

    ofVec3f                 leftArmPntS1;
    ofVec3f                 leftUpperArmPntS1;
    ofVec3f                 leftLowerArmPntS1;

    ofVec3f                 leftLegPntS1;
    ofVec3f                 leftUpperLegPntS1;
    ofVec3f                 leftLowerlegPntS1;

    ofVec3f                 rightArmPntS2;
    ofVec3f                 rightUpperArmPntS2;
    ofVec3f                 rightLowerArmPntS2;

    ofVec3f                 rightLegPntS2;
    ofVec3f                 rightUpperLegPntS2;
    ofVec3f                 rightLowerlegPntS2;

    ofVec3f                 leftArmPntS2;
    ofVec3f                 leftUpperArmPntS2;
    ofVec3f                 leftLowerArmPntS2;

    ofVec3f                 leftLegPntS2;
    ofVec3f                 leftUpperLegPntS2;
    ofVec3f                 leftLowerlegPntS2;


	void close();
	void printStamp() {
		cout << container[currentFrame].stamp << endl;
	}
};
