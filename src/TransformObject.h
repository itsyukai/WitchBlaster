#pragma once
#include "ofMain.h"

//  Kevin M. Smith - CS 134 - SJSU CS
//

//  Base class for any object that needs a transform.
//
class TransformObject {
protected:
	TransformObject();
	ofVec2f position, scale;
	float	rotation;
	bool	bSelected;
public:
	void setPosition(const ofVec3f &);
};
