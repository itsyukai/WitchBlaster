#include "Sprite.h"

BaseObject::BaseObject() {
    trans = ofVec3f(0,0,0);
    scale = ofVec3f(1, 1, 1);
    rot = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
    trans = pos;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
    speed = 0;
    velocity = ofVec3f(0, 0, 0);
    lifespan = -1;      // lifespan of -1 => immortal
    birthtime = 0;
    bSelected = false;
    haveImage = false;
    name = "UnamedSprite";
    width = 20;
    height = 20;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
    return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
    image = img;
    haveImage = true;
    width = image.getWidth();
    height = image.getHeight();
}


// Move and Update velocities based on path
//
void Sprite::update(){
    if(path == Path::SIN){
        ofVec3f temp = ofVec3f(velocity.x,0,0);
        velocity = ofVec3f(velocity.x,sin(ofGetElapsedTimef()) * 100);
        
        /* calcualte velocity angle */
        if (velocity.y < 0)
            rot = temp.angle(velocity);
        else{
            rot = -temp.angle(velocity);
        }
        //cout<<rot<< " : "<<velocity.y<<endl;
    }
    else if (path == Path::COS){
        ofVec3f temp = ofVec3f(velocity.x,0,0);
        velocity = ofVec3f(velocity.x,cos(ofGetElapsedTimef()) * 100);
        
        /* calcualte velocity angle */
        if (velocity.y < 0)
            rot = temp.angle(velocity);
        else{
            rot = -temp.angle(velocity);
        }
    }
    else if (path == Path::STR){
    }
    trans += velocity / ofGetFrameRate();
}


//  Render the sprite
//
void Sprite::draw() {

    ofSetColor(255, 255, 255, 255);

    // draw image centered and add in translation amount
    //
    if (haveImage) {
        ofPushMatrix();
        ofTranslate(-width/2.0 +trans.x, -height/ 2.0+trans.y);
        ofRotate(rot,0,0,1);
        image.draw(0,0);
        //image.draw(-width / 2.0 + trans.x, -height / 2.0 + trans.y);
        ofPopMatrix();
    }
    else {
        // in case no image is supplied, draw something.
        //
        ofNoFill();
        ofSetColor(255, 0, 0);
        ofDrawRectangle(-width / 2.0 + trans.x, -height / 2.0 + trans.y, width, height);
    }
}

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
    s.path = path;
    sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
    sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point
//
bool SpriteSystem::removeNear(ofVec3f point, float dist) {
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

    bool nearRemoved = false;
    while (s != sprites.end()) {
        ofVec3f v = s->trans - point;
        if (v.length() < dist) {
            tmp = sprites.erase(s);
            s = tmp;
            nearRemoved = true;
        }
        else s++;
    }
    return nearRemoved;
}

int SpriteSystem::removeNear(ofVec3f point, float dist, vector<ofVec2f> &loc){
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

    int numRemoved = 0;
    while (s != sprites.end()) {
        ofVec3f v = s->trans - point;
        if (v.length() < dist) {
            loc.push_back(s->trans);
            tmp = sprites.erase(s);
            s = tmp;
            numRemoved++;   /* redundant with loc.size()  REMOVE */
            //cout<<endl << "removeNear "<<loc.size() <<endl;
        }
        else s++;
    }
    return numRemoved;
}

//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

    if (sprites.size() == 0) return;
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

    // check which sprites have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, use an iterator.
    //
    while (s != sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            //            cout << "deleting sprite: " << s->name << endl;
            tmp = sprites.erase(s);
            s = tmp;
        }
        else s++;
    }

    //  Update sprite
    //
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].update();
        //sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
    }
}

//  Render all the sprites
//
void SpriteSystem::draw() {
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].draw();
    }
}
// Sets trig path each sprite will followb
//
void SpriteSystem::setPath(Sprite::Path p){
    path = p;
}
