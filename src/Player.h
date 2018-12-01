//
//  Player.h
//  myShooterPart2
//
//  Created by Yukai Yang on 2/26/18.
//

#include "ofMain.h"
#include "Sprite.h"
#include "Missile.h"
#include "Emitter.h"

class Player {
    public:
        void draw();
        void setup();
        void update();
        void move();
        void updateMissiles();
        void shoot();
    
        void takeDamage();
    
        /* for adjusting rate of fire*/
        // will convert rate of fire into framedelay in future
        void resetCounter();
        int counter;
        const int frameDelay = 6;
    
    
        /* Following http://openframeworks.cc/ofBook/chapters/game_design.html
        key_pressed will only return one key and will not work for multiple key
        inputs like left and up at the same time*/
        bool left_pressed, right_pressed, down_pressed, up_pressed,space_pressed;
    
        Sprite sprite;
        vector<Missile> missiles;
        ofSoundPlayer shootSound;
        int health = 3;
        enum class State {INVULNERABLE,VULNERABLE,SUPER};
        State status;
    
        Emitter *sparkles;
};

