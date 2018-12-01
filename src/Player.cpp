//
//  Player.cpp
//  myShooterPart2
//
//  Created by Terry McGinnis on 2/26/18.
//

#include "Player.h"

// Initial values
void Player::setup(){
    space_pressed = false;
    left_pressed = false;
    right_pressed = false;
    up_pressed = false;
    down_pressed = false;
    
    shootSound.load("sounds/pt.wav");
    ofImage temp;
    temp.load("images/witch.png");
    sprite.setImage(temp);
    temp.load("images/fireball.png");
    
    status = State::VULNERABLE;
    sprite.trans.set(100,320);
    sprite.speed = 10;
    health = 3;
    
    temp.load("images/sparkle.png");
    sparkles = new Emitter(new SpriteSystem);
    sparkles->setChildImage(temp);
    sparkles->setPosition(sprite.trans);
    sparkles->setVelocity(ofVec3f(-200,ofRandom(-50,200),0));
    sparkles->setLifespan(500);
    sparkles->setRate(10);
    sparkles->sys->setPath(Sprite::Path::STR);
    sparkles->start();
}
// draws all the missiles
void Player::draw(){
    sprite.draw();
    sparkles->draw();
    for(auto &m: missiles)
        m.draw();
}

void Player:: update(){
    move();
    shoot();
    updateMissiles();
    sparkles->setPosition(ofVec3f(sprite.trans.x - sprite.image.getWidth()/2.0,sprite.trans.y+20));
    sparkles->setVelocity(ofVec3f(-200,ofRandom(-50,200),0));
    sparkles->update();
}

// Applies movement and then checks for boundaries
void Player::move(){
    ofVec3f dir = ofVec3f(0,0,0);
    
    // sum up vectors
    if(right_pressed){
       dir += ofVec3f(sprite.speed,0,0);
    }
    if(left_pressed){
       dir += ofVec3f(-sprite.speed,0,0);
    }
    if(up_pressed){
       dir += ofVec3f(0,-sprite.speed,0);
    }
    if(down_pressed){
       dir += ofVec3f(0,sprite.speed,0);
    }
    // apply vector
    sprite.trans += dir;
    
    // window border 1136,640 checking
    if (sprite.trans.x > 1136 - sprite.image.getWidth()/2.0) // checks x boundaries
        sprite.trans.x = 1136 - sprite.image.getWidth()/2.0;
    else if (sprite.trans.x < sprite.image.getWidth()/2.0)
        sprite.trans.x = sprite.image.getWidth()/2.0;
    
    if ( sprite.trans.y > 640 - sprite.image.getHeight()/2.0) // checks y boundaries
        sprite.trans.y = 640 - sprite.image.getHeight()/2.0;
    else if (sprite.trans.y < sprite.image.getHeight()/2.0)
        sprite.trans.y = sprite.image.getHeight()/2.0;
}

// Counter is to prevent missiles from being shot at framerate
// rate of fire can be lowered by increasing delay
void Player::shoot(){
   
    if(space_pressed && counter<1){
        // Shoot the missile
        Missile m;
        ofPoint pos = ofPoint(sprite.trans.x + sprite.image.getWidth()/2 , sprite.trans.y);
        m.setup(Missile::Owner::PLAYER, 0, 10, pos);
        missiles.push_back(m);
        shootSound.play();
   
        resetCounter();
    }
    else {
        counter--;
    }
}

/* Updates the Missiles fired by player */
void Player::updateMissiles(){                // Personal Note **
    for(int i=0; i < missiles.size(); i++){   // considered using for(&auto missiles)
        missiles[i].update();                 // could not figure out how to erase missile afterwards
                                              // Switch to vector<Missile>::iterator m later
        
        // window border 1136,640 checking should clean up later
        // trans - extra allows for missile to completely leave screen before being erased
        if (missiles[i].sprite.trans.x - missiles[i].sprite.image.getWidth()/2.0 > 1136)
            missiles.erase(missiles.begin() + i);
    }
}

void Player::resetCounter(){
    counter = frameDelay;
}

void Player::takeDamage(){
    health -=1;
    status = State::INVULNERABLE;
}


