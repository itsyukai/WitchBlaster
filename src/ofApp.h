#pragma once

#include "ofMain.h"
#include "Player.h"
#include "Sprite.h"
#include "Emitter.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"


class ofApp : public ofBaseApp {

    public:
        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    
        void checkCollisions();
        void explode(int numRemoved, vector<ofVec2f> &collisionLocations);
        void handleExplosions();
    
        Player player;
        enum class State {START,GAME,END, PAUSE, GAME_OVER}; /* start, game, end */
        State game_state;
        int level;
        
        ofTrueTypeFont verdana14;
        ofImage background;
    
        ofImage heart1;
        ofImage heart2;
        ofImage heart3;
    
        Emitter *enemySpawner;
        Emitter *enemySpawner2;
    
        vector<Emitter*> enemies;

        int score;
        int timer; /* for invulnerability */
        int gameTimer; /* for levels */
    
        ofPoint mouse_last;
    
        vector<ParticleEmitter*>  explosions;
        vector<ofVec2f> collisionLocations;
};


