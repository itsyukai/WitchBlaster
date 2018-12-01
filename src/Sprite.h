#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Class
// created by Kevin M. Smith - CS 134 SJSU
// edited by Yukai Yang

#include "ofMain.h"

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
    BaseObject();
    ofVec2f trans, scale;
    float    rot;
    bool    bSelected;
    void setPosition(ofVec3f);
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
    Sprite();
    void update();
    void draw();
    float age();
    void setImage(ofImage);
    float speed;    //   in pixels/sec
    ofVec3f velocity; // in pixels/sec
    ofImage image;
    float birthtime; // elapsed time in ms
    float lifespan;  //  time in ms
    string name;
    bool haveImage;
    float width, height;
    
    enum class Path {SIN, COS, STR};
    Path path;
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
    void add(Sprite);
    void remove(int);
    void update();
    bool removeNear(ofVec3f point, float dist);// returns whether it removed anything
    int removeNear(ofVec3f point, float dist, vector<ofVec2f> &locs); // returns numebr of removed and locations
    void draw();
    vector<Sprite> sprites;
    
    void setPath(Sprite::Path p);
    
    Sprite::Path path;
};

