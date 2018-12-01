//
//  Missiles.cpp
//  myShooterPart3
//
//  Created by Yukai Yang on 2/26/18.
//

#include "Missile.h"
#include "ofMain.h"
/* Missiles */
void Missile::setup(Owner o, int d,  float s, ofPoint p){
    owner = o;
    damage = d;
    sprite.speed = s;
    
    sprite.trans.x = p.x;
    sprite.trans.y = p.y;
    
    ofImage temp;
    temp.load("images/fireball.png");
    sprite.setImage(temp);   // Personal Note **
   // sprite.image.load("images/bullet.png");   // depending on owner could ahve different images
                                                // could pass pointer to image stored on owner
}
void Missile::draw() {
    sprite.draw();
}
void Missile::update(){
    move();
}
void Missile:: move(){
    if(owner == Owner::PLAYER)
        sprite.trans.x += sprite.speed;
    
}
