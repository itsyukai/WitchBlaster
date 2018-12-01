//
//  Missiles.h
//  myShooterPart3
//
//  Created by Yukai Yang on 2/26/18.
//

#include "ofMain.h"
#include "Sprite.h"


class Missile {
    public:
        enum class Owner {PLAYER, ENEMY};
        Owner owner;
        void draw();
        void setup(Owner o, int d, float s, ofPoint p);
        void update();  
        void move();
    
        int damage;
        Sprite sprite;
};
