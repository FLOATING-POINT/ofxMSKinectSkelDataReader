#include "ofxMSKinectSkelDataReader.h"
#include <stdio.h>

ofxMSKinectSkelDataReader::ofxMSKinectSkelDataReader(const char *filename) {
	char buf[256];

	sprintf(buf, "%s%s", ofToDataPath("./", true).c_str(), filename);
	rawFile = fopen(buf, "rb");

	if(rawFile != NULL) {

		cout << "ofxMSKinectSkelDataReader::ofxKinectDataReader() | file open success. : " << buf << endl;
		fseek(rawFile, 0L, SEEK_END);
		maxFrame = ftell(rawFile) / sizeof(dataContainer);
		fseek(rawFile, 0L, SEEK_SET);

		container = new dataContainer[maxFrame];
		fread(container, sizeof(dataContainer), maxFrame, rawFile);

		relativeMaxClock = prevClock = maxClock = container[maxFrame - 1].stamp;

		open = true;

	} else {

		cout << "ofxMSKinectSkelDataReader::ofxKinectDataReader() | file open error. : can't open " << buf << endl;
		open = false;
		memset(container, 0, sizeof(dataContainer) * DEPTH_LENGTH);
		maxClock = 1;
	}
	currentFrame        = 0;
	//distancePixels      = container[currentFrame].distancePixels;
	//rgbPixels           = container[currentFrame].rgbPixels;

	pos1                = container[currentFrame].pos1;
    pos2                = container[currentFrame].pos2;

	isStarted           = false;
	startClockOffset    = (double)0;
}
//---------------------------------------------------------------------
bool ofxMSKinectSkelDataReader::start() {
	if(open) {
		isStarted = true;
		startClockOffset = container[currentFrame].stamp + get_stamp_sec();
	}
	return isStarted;
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::stop() {
	isStarted = false;
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::resume() {
	isStarted = true;
}
//---------------------------------------------------------------------
double ofxMSKinectSkelDataReader::readTo() {
	if(open && isStarted) {

		double currentClock = get_stamp_sec() - startClockOffset;
		while(maxClock < currentClock) {
			currentClock -= maxClock;
		}

		//cout << maxFrame << ", " << currentFrame << ", " << maxClock << ", " << currentClock << endl;

		if(currentClock < prevClock) {
			currentFrame = 0;
		}
		while(container[currentFrame].stamp < currentClock) {
			currentFrame++;
			if(maxFrame - 1 <= currentFrame) {
				currentFrame = 0;
				break;
			}
		}

		//distancePixels      = container[currentFrame].distancePixels;
		//rgbPixels           = container[currentFrame].rgbPixels;

		pos1                = container[currentFrame].pos1;
        pos2                = container[currentFrame].pos2;

		prevClock = currentClock;
		return container[currentFrame].stamp;

	} else if(open) {

		//distancePixels      = container[currentFrame].distancePixels;
		//rgbPixels           = container[currentFrame].rgbPixels;
		pos1                = container[currentFrame].pos1;
        pos2                = container[currentFrame].pos2;

		prevClock = startClockOffset + container[currentFrame].stamp;
		return container[currentFrame].stamp;

	} else {
		return (double)0;
	}
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::drawLines(int skelID){

    float * pnts;
    skelID == 0 ? pnts = pos1 : pnts = pos2;

    ofVec3f pos1;
    ofVec3f pos2;
    ofVec3f pos3;
    int id;

    for(int i = 3; i < (20*3); i+=3){

        id = floor(i/3);

        pos1.x = pnts[i-3];
        pos1.y = pnts[i-2];
        pos1.z = pnts[i-1];

        pos2.x = pnts[i];
        pos2.y = pnts[i+1];
        pos2.z = pnts[i+2];

        ofSetHexColor(0x000000);
        if( id != 4 && id != 8 && id != 12 && id != 16) ofLine(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);

    }

    // across the shoulders

    // point 4
    pos1.x = pnts[12];
    pos1.y = pnts[12+1];
    pos1.z = pnts[12+2];

    // point 8
    pos2.x = pnts[24];
    pos2.y = pnts[24+1];
    pos2.z = pnts[24+2];

    ofLine(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);

    // across the hips

    // point 0
    pos1.x = pnts[0];
    pos1.y = pnts[1];
    pos1.z = pnts[2];

    // point 12
    pos2.x = pnts[36];
    pos2.y = pnts[36+1];
    pos2.z = pnts[36+2];

    // point 16
    pos3.x = pnts[48];
    pos3.y = pnts[48+1];
    pos3.z = pnts[48+2];

    ofLine(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z);
    ofLine(pos1.x, pos1.y, pos1.z, pos3.x, pos3.y, pos3.z);

}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::drawPoints(int skelID){

    float * pnts;
    skelID == 0 ? pnts = pos1 : pnts = pos2;

    int id;

    for(int i = 0; i < (20*3); i+=3){ //(20*3); 20 points with x,y,z

        id = floor(i/3);

        ofSetHexColor(0xff0000);
        ofRect(pnts[i]-8, pnts[i+1]-8, pnts[i+2], 16,16);

        ofSetHexColor(0xffffff);
        ofDrawBitmapString(ofToString(id), pnts[i]-8 ,pnts[i+1]+8, pnts[i+2]);

    }

}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::drawCirclePoints(int skelID){

    float * pnts;
    skelID == 0 ? pnts = pos1 : pnts = pos2;

    int id;

    for(int i = 0; i < (20*3); i+=3){ //(20*3); 20 points with x,y,z

        id = floor(i/3);

        if(id == 6 || id == 10 || id == 18 || id == 14){

            ofSetHexColor(0x0000ff);
            ofCircle(pnts[i], pnts[i+1], pnts[i+2], 6);

        } else {

            ofSetHexColor(0xff0000);
            ofCircle(pnts[i], pnts[i+1], pnts[i+2], 6);

        }

    }

}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::drawLimbPoints(int skelID){



    if(skelID == 0){

        ofFill();
        ofSetHexColor(0xffffff);
        ofCircle(rightUpperArmPntS1.x, rightUpperArmPntS1.y, rightUpperArmPntS1.z, 4);
        ofCircle(rightUpperLegPntS1.x, rightUpperLegPntS1.y, rightUpperLegPntS1.z, 4);

        ofSetHexColor(0xFF8000);
        ofCircle(rightLowerArmPntS1.x, rightLowerArmPntS1.y, rightLowerArmPntS1.z, 4);
        ofCircle(rightLowerlegPntS1.x, rightLowerlegPntS1.y, rightLowerlegPntS1.z, 4);

        ofSetHexColor(0xffffff);
        ofCircle(leftUpperArmPntS1.x, leftUpperArmPntS1.y, leftUpperArmPntS1.z, 4);
        ofCircle(leftUpperLegPntS1.x, leftUpperLegPntS1.y, leftUpperLegPntS1.z, 4);

        ofSetHexColor(0xFF8000);
        ofCircle(leftLowerArmPntS1.x, leftLowerArmPntS1.y, leftLowerArmPntS1.z, 4);
        ofCircle(leftLowerlegPntS1.x, leftLowerlegPntS1.y, leftLowerlegPntS1.z, 4);

        ofNoFill();
        ofSetHexColor(0xCCCCCC);
        ofCircle(leftArmPntS1.x,    leftArmPntS1.y,     leftArmPntS1.z, 4);
        ofCircle(rightArmPntS1.x,   rightArmPntS1.y,    rightArmPntS1.z, 4);
        ofCircle(leftLegPntS1.x,    leftLegPntS1.y,     leftLegPntS1.z, 4);
        ofCircle(rightLegPntS1.x,   rightLegPntS1.y,    rightLegPntS1.z, 4);

    }

    if(skelID == 1){

        ofFill();
        ofSetHexColor(0xffffff);

        ofCircle(rightUpperArmPntS2.x, rightUpperArmPntS2.y, rightUpperArmPntS2.z, 4);
        ofCircle(rightUpperLegPntS2.x, rightUpperLegPntS2.y, rightUpperLegPntS2.z, 4);

        ofSetHexColor(0xFF8000);
        ofCircle(rightLowerArmPntS2.x, rightLowerArmPntS2.y, rightLowerArmPntS2.z, 4);
        ofCircle(rightLowerlegPntS2.x, rightLowerlegPntS2.y, rightLowerlegPntS2.z, 4);

        ofSetHexColor(0xffffff);
        ofCircle(leftUpperArmPntS2.x, leftUpperArmPntS2.y, leftUpperArmPntS2.z, 4);
        ofCircle(leftUpperLegPntS2.x, leftUpperLegPntS2.y, leftUpperLegPntS2.z, 4);

        ofSetHexColor(0xFF8000);
        ofCircle(leftLowerArmPntS2.x, leftLowerArmPntS2.y, leftLowerArmPntS2.z, 4);
        ofCircle(leftLowerlegPntS2.x, leftLowerlegPntS2.y, leftLowerlegPntS2.z, 4);

        ofNoFill();
        ofSetHexColor(0xCCCCCC);
        ofCircle(leftArmPntS2.x,    leftArmPntS2.y,     leftArmPntS2.z, 4);
        ofCircle(rightArmPntS2.x,   rightArmPntS2.y,    rightArmPntS2.z, 4);
        ofCircle(leftLegPntS2.x,    leftLegPntS2.y,     leftLegPntS2.z, 4);
        ofCircle(rightLegPntS2.x,   rightLegPntS2.y,    rightLegPntS2.z, 4);
    }


}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::updatePnts() {


    rightArmPntS1           = getRightArmPoint(0);
    rightUpperArmPntS1      = getRightUpperArmPoint(0);
    rightLowerArmPntS1      = getRightLowerArmPoint(0);

    rightLegPntS1           = getRightLegPoint(0);
    rightUpperLegPntS1      = getRightUpperLegPoint(0);
    rightLowerlegPntS1      = getRightLowerLegPoint(0);

    leftArmPntS1            = getLeftArmPoint(0);
    leftUpperArmPntS1       = getLeftUpperArmPoint(0);
    leftLowerArmPntS1       = getLeftLowerArmPoint(0);

    leftLegPntS1            = getLeftLegPoint(0);
    leftUpperLegPntS1       = getLeftUpperLegPoint(0);
    leftLowerlegPntS1       = getLeftLowerLegPoint(0);

    rightArmPntS2           = getRightArmPoint(1);
    rightUpperArmPntS2      = getRightUpperArmPoint(1);
    rightLowerArmPntS2      = getRightLowerArmPoint(1);

    rightLegPntS2           = getRightLegPoint(1);
    rightUpperLegPntS2      = getRightUpperLegPoint(1);
    rightLowerlegPntS2      = getRightLowerLegPoint(1);

    leftArmPntS2            = getLeftArmPoint(1);
    leftUpperArmPntS2       = getLeftUpperArmPoint(1);
    leftLowerArmPntS2       = getLeftLowerArmPoint(1);

    leftLegPntS2            = getLeftLegPoint(1);
    leftUpperLegPntS2       = getLeftUpperLegPoint(1);
    leftLowerlegPntS2       = getLeftLowerLegPoint(1);
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::shift(int ds) {

	double dClock       = container[currentFrame].stamp;
	currentFrame        = (maxFrame + currentFrame + ds) % maxFrame;
	startClockOffset    += container[currentFrame].stamp - dClock;
	prevClock           = get_stamp_sec() - startClockOffset;

	cout << "shift " << ds << "frame | now " << currentFrame << "frame." << endl;
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::nextFrame() {

    currentFrame++;
	doFrameMove();
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::prevFrame() {

    currentFrame--;
    doFrameMove();
}
//---------------------------------------------------------------------
void ofxMSKinectSkelDataReader::doFrameMove() {

	double dClock       = container[currentFrame].stamp;
	currentFrame        = (maxFrame + currentFrame) % maxFrame;
	startClockOffset    += container[currentFrame].stamp - dClock;
	prevClock           = get_stamp_sec() - startClockOffset;

	//cout << "frame | now " << currentFrame << "frame." << endl;
}
float * ofxMSKinectSkelDataReader::getPoints(int skelID){

   if(skelID == 0){
       return pos1;
    } else {
        return pos2;
    };

}

//---------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getPoint(int skelid, int pntID){ // point id 19

    ofVec3f pnt;
    int id;

    if(skelid == 0){
        id = pntID*3;
        pnt.x = pos1[id];
        pnt.y = pos1[id+1];
        pnt.z = pos1[id+2];
        return pnt;
    } else {
        id = pntID*3;
        pnt.x = pos2[id];
        pnt.y = pos2[id+1];
        pnt.z = pos2[id+2];
        return pnt;
    }

}
//---------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getGroinPoint(int skelid){ // point id 0

    int id = 0;
    return getPoint(skelid, id);

}
//---------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getStomachPoint(int skelid){ // point id 1

    int id = 1;
    return getPoint(skelid, id);


}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getNeckPoint(int skelid){ // point id 2

    int id = 2;
    return getPoint(skelid, id);


}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getHeadPoint(int skelid){ // point id 3

    int id = 3;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftShoulderPoint(int skelid){ // point id 4

    int id = 4;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftElbowPoint(int skelid){// point id 5

    int id = 5;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftWristPoint(int skelid){ // point id 6

    int id = 6;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftHandPoint(int skelid){ // point id 7

    int id = 7;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightShoulderPoint(int skelid){ // point id 8

    int id = 8;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightElbowPoint(int skelid){ // point id 9

    int id = 9;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightWristPoint(int skelid){ // point id 10

    int id = 10;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightHandPoint(int skelid){ // point id 11

    int id = 11;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftHipPoint(int skelid){ // point id 12

    int id = 12;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftKneePoint(int skelid){ // point id 13

    int id = 13;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftAnklePoint(int skelid){  // point id 14

    int id = 14;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftFootPoint(int skelid){ // point id 15

    int id = 15;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightHipPoint(int skelid){ // point id 16

    int id = 16;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightKneePoint(int skelid){ // point id 17

    int id = 17;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightAnklePoint(int skelid){ // point id 18

    int id = 18;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightToePoint(int skelid){ // point id 19

    int id = 19;
    return getPoint(skelid, id);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLimbPoint(int skelid, int posID1, int posID2){

    int id1 = posID1;
    int id2 = posID2;

    ofVec3f pntPos;
    ofVec3f pntPos1;
    ofVec3f pntPos2;

	float * skelPos;


    if(skelid == 0){

        skelPos = pos1;

    } else {
        skelPos = pos2;
    }

    int startPos1 = id1*3;
    pntPos1.x     = skelPos[startPos1];
    pntPos1.y     = skelPos[startPos1+1];
    pntPos1.z     = skelPos[startPos1+2];

    int startPos2 = id2*3;
    pntPos2.x     = skelPos[startPos2];
    pntPos2.y     = skelPos[startPos2+1];
    pntPos2.z     = skelPos[startPos2+2];

    //-------------------------------------------------------

    pntPos.x          = pntPos1.x + ((pntPos2.x - pntPos1.x )*.5);
    pntPos.y          = pntPos1.y + ((pntPos2.y - pntPos1.y )*.5);
    pntPos.z          = pntPos1.z + ((pntPos2.z - pntPos1.z )*.5);


    return pntPos;

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLimbPairPoint(int skelid, int posID1, int posID2,int posID3){

    int id1 = posID1;
    int id2 = posID2;
    int id3 = posID3;

    ofVec3f pntPos;
    ofVec3f pntPos1;
    ofVec3f pntPos2;
    ofVec3f pntPos3;

	float * skelPos;


    if(skelid == 0){

        skelPos = pos1;

    } else {
        skelPos = pos2;
    }

    int startPos1 = id1*3;
    pntPos1.x     = skelPos[startPos1];
    pntPos1.y     = skelPos[startPos1+1];
    pntPos1.z     = skelPos[startPos1+2];

    int startPos2 = id2*3;
    pntPos2.x     = skelPos[startPos2];
    pntPos2.y     = skelPos[startPos2+1];
    pntPos2.z     = skelPos[startPos2+2];

    int startPos3 = id3*3;
    pntPos3.x     = skelPos[startPos3];
    pntPos3.y     = skelPos[startPos3+1];
    pntPos3.z     = skelPos[startPos3+2];

    //-------------------------------------------------------

    pntPos.x          = ((pntPos1.x + pntPos2.x + pntPos3.x )/3.0f) ;
    pntPos.y          = ((pntPos1.y + pntPos2.y + pntPos3.y )/3.0f) ;
    pntPos.z          = ((pntPos1.z + pntPos2.z + pntPos3.z )/3.0f) ;

    return pntPos;

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightArmPoint(int skelid){
    int id1 = 8;
    int id2 = 9;
    int id3 = 10;
    return getLimbPairPoint(skelid, id1,id2,id3);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightUpperArmPoint(int skelid){

    int id1 = 8;
    int id2 = 9;
    return getLimbPoint(skelid, id1,id2);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightLowerArmPoint(int skelid){

    int id1 = 9;
    int id2 = 10;
    return getLimbPoint(skelid, id1,id2);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftArmPoint(int skelid){
    int id1 = 4;
    int id2 = 5;
    int id3 = 6;
    return getLimbPairPoint(skelid, id1,id2,id3);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftUpperArmPoint(int skelid){

    int id1 = 4;
    int id2 = 5;
    return getLimbPoint(skelid, id1,id2);

}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftLowerArmPoint(int skelid){
    int id1 = 5;
    int id2 = 6;
    return getLimbPoint(skelid, id1,id2);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightLegPoint(int skelid){
    int id1 = 16;
    int id2 = 17;
    int id3 = 18;
    return getLimbPairPoint(skelid, id1,id2,id3);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightUpperLegPoint(int skelid){
    int id1 = 16;
    int id2 = 17;
    return getLimbPoint(skelid, id1,id2);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getRightLowerLegPoint(int skelid){
    int id1 = 17;
    int id2 = 18;
    return getLimbPoint(skelid, id1,id2);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftLegPoint(int skelid){
    int id1 = 12;
    int id2 = 13;
    int id3 = 14;
    return getLimbPairPoint(skelid, id1,id2,id3);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftUpperLegPoint(int skelid){
    int id1 = 12;
    int id2 = 13;
    return getLimbPoint(skelid, id1,id2);
}
//-------------------------------------------------------------------------
ofVec3f ofxMSKinectSkelDataReader::getLeftLowerLegPoint(int skelid){
    int id1 = 13;
    int id2 = 14;
    return getLimbPoint(skelid, id1,id2);
}
//-------------------------------------------------------------------------

/*
void ofxMSKinectSkelDataReader::asignDistancePixels(float ** _distancePixels) {
	*_distancePixels = distancePixels;
}

float *ofxMSKinectSkelDataReader::getDistancePixels() {
	return distancePixels;
}

void ofxMSKinectSkelDataReader::asignRgbPixels(unsigned char** _rgbPixels) {
	*_rgbPixels = rgbPixels;
}

unsigned char *ofxMSKinectSkelDataReader::getRGBPixels() {
	return rgbPixels;
}
*/
void ofxMSKinectSkelDataReader::close() {
	if(open) {
		fclose(rawFile);
		delete [] container;
		open = false;
		cout << "ofxMSKinectSkelDataReader::close() | success." << endl;
	} else {
		cout << "ofxMSKinectSkelDataReader::close() | don't open file." << endl;
	}
}
